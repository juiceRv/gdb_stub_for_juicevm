#include<stdio.h>
#include<sys/socket.h> 
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <string.h>
#define MAX_LISTEN 1

extern int Socket(int domain, int type, int protocol);
extern void Bind(int sockfd, struct sockaddr *myaddr, int addrlen);
extern void Listen(int sockfd, int backlog);
extern int Accept(int listenfd, struct sockaddr *addr, int *addrlen);
extern void Close(int clientfd);
extern struct hostent* Gethostbyaddr(const char *addr, int len, int domain);
extern ssize_t Read(int fd, void* buf, size_t n);
extern ssize_t Write(int fd, const void* buf, size_t n);
extern void echo(int listenfd);
 
int main(int argc, char **argv){
    int servfd, clientfd, port, clientlen;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    struct hostent *host;
    char* hostaddr;
    if ( argc != 2){
        fprintf(stderr,"usage:%s<port>\n", argv[0]);
        exit(0);
    }
    juicevm_cpu_set_tick_cb(juicevm_tick_cb_func);
    juicevm_cpu_set_check_cb(juicevm_cpu_check_mem_and_pc_cb_func);
    port = atoi(argv[1]);  // get port
 
    servfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    // init servaddr
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons((unsigned short)port);
    clientlen = sizeof(cliaddr);
    
    Bind(servfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(servfd, MAX_LISTEN);
 
    while(1){   // init server
        memset(&cliaddr, 0, sizeof(cliaddr));
        clientfd = Accept(servfd, (struct sockaddr*)&cliaddr, &clientlen);
        host = Gethostbyaddr((const char*)&cliaddr.sin_addr.s_addr, sizeof(cliaddr.sin_addr.s_addr), AF_INET);
        printf("server connect to host: %s %s\n",host->h_name, inet_ntoa(cliaddr.sin_addr));
        echo(clientfd);
        Close(clientfd);
    }
}