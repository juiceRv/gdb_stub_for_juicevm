#ifndef __GDB_API_H_
#define __GDB_API_H_

#include <stdlib.h>
enum {
    GDB_SIGNAL_0 = 0,
    GDB_SIGNAL_INT = 2,
    GDB_SIGNAL_QUIT = 3,
    GDB_SIGNAL_TRAP = 5,
    GDB_SIGNAL_ABRT = 6,
    GDB_SIGNAL_ALRM = 14,
    GDB_SIGNAL_IO = 23,
    GDB_SIGNAL_XCPU = 24,
    GDB_SIGNAL_UNKNOWN = 143
};
void gdb_ext_api_stop_cpu(void);
void gdb_ext_api_start_cpu(void);
void gdb_ext_api_cpu_set_cpu_pc(uint64_t addr);
void gdb_ext_api_stub_exit(void);

#if ( defined(__CPU_ADDR_XLEN_) && __CPU_ADDR_XLEN_ == 8)
volatile uint64_t gdb_cmd_func_ext_api_cpu_read_reg(volatile uint64_t addr);
void gdb_cmd_func_ext_api_cpu_write_reg(volatile uint64_t addr,volatile uint64_t val);
volatile void gdb_cmd_func_ext_api_cpu_read_mem(volatile uint64_t addr,uint8_t *out,uint64_t len);

volatile void gdb_cmd_func_ext_api_cpu_write_mem(volatile uint64_t addr,uint8_t *in,uint64_t len);
#endif

static inline int gdb_ext_api_tohex(int v)
{
    if (v < 10)
        return v + '0';
    else
        return v - 10 + 'a';
}
static inline int gdb_ext_api_fromhex(int v)
{
    if (v >= '0' && v <= '9')
        return v - '0';
    else if (v >= 'A' && v <= 'F')
        return v - 'A' + 10;
    else if (v >= 'a' && v <= 'f')
        return v - 'a' + 10;
    else
        return 0;
}
#endif // __GDB_API_H_