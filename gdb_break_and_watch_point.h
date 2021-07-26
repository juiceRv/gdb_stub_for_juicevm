#ifndef __GDB_BREAK_AND_WATCH_POINT_H_
#define __GDB_BREAK_AND_WATCH_POINT_H_
#include "inttypes.h"
#include <string.h>
#include <stdio.h>

#ifndef __GDB_MAX_BREAKPOINTS_NUM_
#define __GDB_MAX_BREAKPOINTS_NUM_    100
#endif

#ifndef __GDB_MAX_WATCHPOINTS_NUM_
#define __GDB_MAX_WATCHPOINTS_NUM_    100
#endif

typedef enum{
    GDB_CHECK_TYPE_BKP = 0,
    GDB_CHECK_TYPE_WTP = 1
}gdb_check_t;

typedef enum{
    GDB_MEM_OPTYPE_READ = 0,
    GDB_MEM_OPTYPE_WRITE = 1,
}gdb_mem_optype_t;

#define insert_break_or_watchpoint_software_breakpoint    0
#define insert_break_or_watchpoint_hardware_breakpoint    1
#define insert_break_or_watchpoint_write_watchpoint       2
#define insert_break_or_watchpoint_read_watchpoint        3
#define insert_break_or_watchpoint_access_watchpoint      4

void gdb_break_and_watch_point_check(gdb_check_t check_type,volatile uint64_t pc,volatile uint64_t mem_addr,gdb_mem_optype_t mem_optype,volatile uint64_t len);
uint8_t gdb_breakpoint_add(volatile uint8_t type,volatile uint64_t addr,volatile uint64_t len);
void gdb_breakpoint_del(volatile uint8_t type,volatile uint64_t addr,volatile uint64_t len);
uint8_t gdb_watchpoint_add(volatile uint8_t type,volatile uint64_t addr,volatile uint64_t len);
void gdb_watchpoint_del(volatile uint8_t type,volatile uint64_t addr,volatile uint64_t len);

#endif // __GDB_BREAK_AND_WATCH_POINT_H_