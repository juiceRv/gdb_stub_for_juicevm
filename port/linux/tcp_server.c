#include<stdio.h>
#include<sys/socket.h> 
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/ioctl.h>

#include "../../gdb_stub.h"
#include "../../gdb_break_and_watch_point.h"

#define MAX_LISTEN 1024
#define MAX_LINE 1024
 
int Socket(int domain, int type, int protocol){
    int sockfd = socket(domain, type, protocol);
    if ( sockfd < 0 ){
        printf("init socket failed: %d \n",sockfd);
        exit(0);
    }
    return sockfd;
}
 
void Bind(int sockfd, struct sockaddr *myaddr, int addrlen){
    if ( bind(sockfd, myaddr, addrlen) < 0 ){
        printf("bind\n");
        exit(0);
    } }
 
void Listen(int sockfd, int backlog){
    if ( listen(sockfd, backlog) < 0){
        printf("listen\n");
        exit(0);
    }
}
 
int Accept(int listenfd, struct sockaddr *addr, int *addrlen){
    int clientfd = accept(listenfd, addr, addrlen);
    if ( clientfd < 0){
        printf("accept\n");
        exit(0);
    }
    return clientfd;
}
 
void Close(int clientfd){
    if ( close(clientfd) < 0){
        printf("close\n");
        exit(0);
    }
}
struct hostent* Gethostbyaddr(const char *addr, int len, int domain){
    struct hostent* host = gethostbyaddr(addr, len, domain);
    if ( NULL == host ){
        printf("host_by_addr\n");
        exit(0);
    }
    return host;
}
 
ssize_t Read(int fd, void* buf, size_t n){
    ssize_t num= recv(fd, buf, n,0);
    if ( n < 0){
        printf("read\n");
        exit(0);
    }
    return num;
}
 
ssize_t Write(int fd, const void* buf, size_t n){
    ssize_t num = write(fd, buf, n);
    if ( n < 0){
        printf("write\n");
        exit(0);
    }
    return num;
}
int listenfd_global;
void echo(int listenfd){
    ssize_t n;
    char read_buff[100];
    char *read_buff_p;read_buff_p = read_buff;

    listenfd_global = listenfd;
    // while(1){
        n = read(listenfd, read_buff, 100);
        // printf("recv: %d %.*s\n",n,n,read_buff);
        if(n > 0){
            // printf("recv: %d %.*s\n",n,n,read_buff);
            // printf("recv: %d %s\n",n,&(read_buff[0]));
            while(n--)
            {
                gdb_ext_api_deal_char(*(read_buff_p++));
            }
            
            
        }else if(n < 0){
            return;
        }
        // n = write(listenfd, write_buff, MAX_LINE);
    
        
    // }

}

int servfd, clientfd, clientlen;
void juicevm_tick_cb_func(void){
    echo(clientfd);
}

void juicevm_cpu_check_mem_and_pc_cb_func(gdb_check_t check_type,volatile uint64_t pc,volatile uint64_t mem_addr,gdb_mem_optype_t mem_optype,volatile uint64_t len) {
    gdb_break_and_watch_point_check(check_type,pc,mem_addr,mem_optype,len);
}

void gdb_stub_tcp_server_exit(void)
{
    Close(clientfd);
}
void gdb_stub_tcp_server(const char * addr,const uint16_t port){
    
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    struct hostent *host;
    char* hostaddr;
    if(!addr){
        return;
    }
    // port = 12345;  // get port
 
    juicevm_cpu_set_tick_cb(juicevm_tick_cb_func);
    juicevm_cpu_set_check_cb(juicevm_cpu_check_mem_and_pc_cb_func);

    servfd = Socket(AF_INET, SOCK_STREAM, 0);

    // init servaddr
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr(addr);
    servaddr.sin_port = htons((unsigned short)port);
    clientlen = sizeof(cliaddr);
    
    Bind(servfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(servfd, MAX_LISTEN);
 
    // while(1){   // init server
        memset(&cliaddr, 0, sizeof(cliaddr));
        printf("listen port %d\n",port);
        clientfd = Accept(servfd, (struct sockaddr*)&cliaddr, &clientlen);
        host = Gethostbyaddr((const char*)&cliaddr.sin_addr.s_addr, sizeof(cliaddr.sin_addr.s_addr), AF_INET);
        printf("server connect to host: %s %s\n",host->h_name, inet_ntoa(cliaddr.sin_addr));

        // struct timeval timeout = {0,0}; 
        // setsockopt(clientfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(struct timeval));

        fcntl(clientfd, F_SETFL, fcntl(clientfd, F_GETFL, 0) | O_NONBLOCK);
        ioctl(clientfd, FIONBIO, 1); //1:非阻塞 0:阻塞

        // echo(clientfd);
        // Close(clientfd);
    // }
}