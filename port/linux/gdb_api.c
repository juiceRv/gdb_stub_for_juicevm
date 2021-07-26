#include "../../gdb_stub.h"
extern int listenfd_global;
void gdb_ext_api_send_char(const char c)
{
    // printf("%s->%s:%d buf_len:%c",__FILE__,__FUNCTION__,__LINE__,c);
    write(listenfd_global, &c, 1);
}

void gdb_ext_api_send_buf(const char *buf, int len)
{
    // printf("%s->%s:%d buf:%s\n",__FILE__,__FUNCTION__,__LINE__,buf);
    while(len--) {
        // printf("%s->%s:%d buf_len:%d\n",__FILE__,__FUNCTION__,__LINE__,len);
        gdb_ext_api_send_char(*(buf++));
    }
}

int gdb_ext_api_isdigit(int c)
{
	return ((unsigned)c - '0') < 10;
}

int gdb_ext_api_isxdigit(int c)
{
	return gdb_ext_api_isdigit(c) || (((unsigned)c | 32) - 'a' < 6);
}

uint8_t gdb_ext_api_cpustate_is_running(void)
{
    return juicevm_get_cpu_state();
    // return 0; // pause
    // return 1; // running
}

void gdb_ext_api_stop_cpu(void)
{
    // printf("gdb_ext_api_stop_cpu\n");
    juicevm_set_cpu_stop();
    // gdb_ext_api_trap_packet_send("S02");gdb_ext_api_deal_char("");

}

void gdb_ext_api_start_cpu(void)
{
    // printf("gdb_ext_api_start_cpu\n");
    juicevm_set_cpu_start();
}

extern void gdb_ext_api_deal_char(const char ch);


void gdb_ext_api_stub_exit(void){
    gdb_stub_tcp_server_exit();
}