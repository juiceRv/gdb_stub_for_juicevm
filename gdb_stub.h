#ifndef __GDB_STUB_H_
#define __GDB_STUB_H_
#include "inttypes.h"
#include <string.h>
#include <stdio.h>
#define __CPU_MAX_PACKET_LENGTH_  1024
#ifndef __CPU_MAX_PACKET_LENGTH_
#error "__CPU_MAX_PACKET_LENGTH_ is not set";
#endif
#define __CPU_ADDR_XLEN_  8
#ifndef __CPU_ADDR_XLEN_
#error "__CPU_ADDR_XLEN_ is not set";
#endif
#define __CPU_REGS_NUM_  33
#ifndef __CPU_REGS_NUM_
#error "__CPU_REGS_NUM_ is not set";
#endif
typedef enum gdb_rs_state {
    GDB_RS_IDLE,
    GDB_RS_GETLINE,
    GDB_RS_GETLINE_ESC,
    GDB_RS_GETLINE_RLE,
    GDB_RS_CHKSUM1,
    GDB_RS_CHKSUM2,
    GDB_RS_WAIT_RSPON,
}gdb_rs_state_t;
int gdb_handle_packet(const char *buf,const uint16_t len);
void gdb_ext_api_put_packet_binary(const char *buf, int len);
void gdb_ext_api_put_packet(const char *buf);
void gdb_ext_api_deal_char(const char ch);
void gdb_ext_api_send_buf(const char *buf, int len);
uint8_t gdb_ext_api_cpustate_is_running(void);
void gdb_ext_api_trap_packet_send(const char *buf);
#endif // __GDB_STUB_H_