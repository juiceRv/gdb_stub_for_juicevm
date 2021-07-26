#include "gdb_stub.h"
#include "gdb_api.h"
#include "gdb_break_and_watch_point.h"
typedef struct{
    volatile uint64_t en;
    volatile uint8_t  type;
    volatile uint64_t addr;
    volatile uint64_t len;
}gdb_bkp_or_watch_t;

static gdb_bkp_or_watch_t gdb_bkp_buf[__GDB_MAX_BREAKPOINTS_NUM_];
// {
//     [0 ... GDB_MAX_BREAKPOINTS-1] = { .state = BP_UNDEFINED }
// };
static gdb_bkp_or_watch_t gdb_watch_buf[__GDB_MAX_WATCHPOINTS_NUM_];

void gdb_break_and_wathc_point_mag_init(void){
    memset(gdb_bkp_buf,0,sizeof(gdb_bkp_buf));
}

// if breakpoint list is full return 0;
uint8_t gdb_breakpoint_add(volatile uint8_t type,volatile uint64_t addr,volatile uint64_t len){
    uint16_t i;
    for(i = 0; i < __GDB_MAX_BREAKPOINTS_NUM_; i++){
        if(!gdb_bkp_buf[i].en){
            gdb_bkp_buf[i].en = 1;
            gdb_bkp_buf[i].type = type;
            gdb_bkp_buf[i].addr = addr;
            gdb_bkp_buf[i].len = len;
            return 1;
        }
    }
    return 0;
}

void gdb_breakpoint_del(volatile uint8_t type,volatile uint64_t addr,volatile uint64_t len){
    uint16_t i;
    for(i = 0; i < __GDB_MAX_BREAKPOINTS_NUM_; i++){
        if(gdb_bkp_buf[i].en && gdb_bkp_buf[i].type == type && gdb_bkp_buf[i].addr == addr && gdb_bkp_buf[i].len == len){
            gdb_bkp_buf[i].en = 0;
        }
    }
}

// if breakpoint list is full return 0;
uint8_t gdb_watchpoint_add(volatile uint8_t type,volatile uint64_t addr,volatile uint64_t len){
    uint16_t i;
    for(i = 0; i < __GDB_MAX_WATCHPOINTS_NUM_; i++){
        if(!gdb_watch_buf[i].en){
            gdb_watch_buf[i].en = 1;
            gdb_watch_buf[i].type = type;
            gdb_watch_buf[i].addr = addr;
            gdb_watch_buf[i].len = len;
            return 1;
        }
    }
    return 0;
}

void gdb_watchpoint_del(volatile uint8_t type,volatile uint64_t addr,volatile uint64_t len){
    uint16_t i;
    for(i = 0; i < __GDB_MAX_WATCHPOINTS_NUM_; i++){
        if(gdb_watch_buf[i].en && gdb_watch_buf[i].type == type && gdb_watch_buf[i].addr == addr && gdb_watch_buf[i].len == len){
            gdb_watch_buf[i].en = 0;
        }
    }
}

void gdb_break_and_watch_point_check(gdb_check_t check_type,volatile uint64_t pc,volatile uint64_t mem_addr,gdb_mem_optype_t mem_optype,volatile uint64_t len){
    uint16_t i;
    char tmp_str_buf[] = "T05awatch:0123456789abcedf;";

    switch(check_type){
        case GDB_CHECK_TYPE_BKP:
            for(i = 0; i < __GDB_MAX_BREAKPOINTS_NUM_; i++){
                if(gdb_bkp_buf[i].en && gdb_bkp_buf[i].addr == pc){
                    if(gdb_watch_buf[i].type == insert_break_or_watchpoint_software_breakpoint){
                        gdb_ext_api_trap_packet_send("S05");gdb_ext_api_deal_char("");
                        gdb_ext_api_stop_cpu();
                    }else if(gdb_watch_buf[i].type == insert_break_or_watchpoint_hardware_breakpoint){
                        gdb_ext_api_trap_packet_send("S05");gdb_ext_api_deal_char("");
                        gdb_ext_api_stop_cpu();
                    }
                    
                }
            }
        break;
        case GDB_CHECK_TYPE_WTP:
            for(i = 0; i < __GDB_MAX_WATCHPOINTS_NUM_; i++){
                if(gdb_watch_buf[i].en && gdb_watch_buf[i].addr >= mem_addr && gdb_watch_buf[i].addr <= (mem_addr+len)){
                    switch(gdb_watch_buf[i].type){
                        case insert_break_or_watchpoint_write_watchpoint:
                            if(mem_optype == GDB_MEM_OPTYPE_WRITE){
                                sprintf(tmp_str_buf,"T05watch:%llx;",mem_addr);
                                gdb_ext_api_trap_packet_send(tmp_str_buf);gdb_ext_api_deal_char("");
                                gdb_ext_api_stop_cpu();
                            }
                        break;
                        case insert_break_or_watchpoint_read_watchpoint:
                            if(mem_optype == GDB_MEM_OPTYPE_READ){
                                sprintf(tmp_str_buf,"T05rwatch:%llx;",mem_addr);
                                gdb_ext_api_trap_packet_send(tmp_str_buf);gdb_ext_api_deal_char("");
                                gdb_ext_api_stop_cpu();
                            }
                        break;
                        case insert_break_or_watchpoint_access_watchpoint:
                            sprintf(tmp_str_buf,"T05awatch:%llx;",mem_addr);
                            gdb_ext_api_trap_packet_send(tmp_str_buf);gdb_ext_api_deal_char("");
                            gdb_ext_api_stop_cpu();
                        break;
                        default:

                        break;

                    }
                    
                    
                }
            }
        break;
        default:

        break;
    }
}