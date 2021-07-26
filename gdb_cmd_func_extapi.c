
#include "gdb_stub.h"
unsigned char gdb_cmd_func_ext_api_cpu_get_cpu_pid(void){
    return 0x00;
}

typedef struct{
    uint64_t x0_zero; // Hardware wired zero line
    uint64_t x1_ra; // Return address
    uint64_t x2_sp; // Stack  pointer
    uint64_t x3_gp; // Global pointer
    uint64_t x4_tp; // Thread pointer
    uint64_t x5_t0; // Temporary
    uint64_t x6_t1; // Temporary
    uint64_t x7_t2; // Temporary
    uint64_t x8_s0_fp; // Saved register,frame_pointer
    uint64_t x9_s1; // Saved register
    uint64_t x10_a0; // Funtion argument,return value
    uint64_t x11_a1; // Funtion argument,return value
    uint64_t x12_a2; // Funtion argument
    uint64_t x13_a3; // Funtion argument
    uint64_t x14_a4; // Funtion argument
    uint64_t x15_a5; // Funtion argument
    uint64_t x16_a6; // Funtion argument
    uint64_t x17_a7; // Funtion argument
    uint64_t x18_s2; // Saved register
    uint64_t x19_s3; // Saved register
    uint64_t x20_s4; // Saved register
    uint64_t x21_s5; // Saved register
    uint64_t x22_s6; // Saved register
    uint64_t x23_s7; // Saved register
    uint64_t x24_s8; // Saved register
    uint64_t x25_s9; // Saved register
    uint64_t x26_s10;// Saved register
    uint64_t x27_s11;// Saved register
    uint64_t x28_t3; // Temporary
    uint64_t x29_t4; // Temporary
    uint64_t x30_t5; // Temporary
    uint64_t x31_t6; // Temporary
    uint64_t pc;
}rv_cpu_reg_t;
volatile rv_cpu_reg_t tmp_reg;
#if ( defined(__CPU_ADDR_XLEN_) && __CPU_ADDR_XLEN_ == 8)
volatile uint64_t gdb_cmd_func_ext_api_cpu_read_reg(volatile uint64_t addr){
    // tmp_reg.pc = (uint64_t)(0x123456789abcdef1);
    // volatile uint64_t tmp_val = (uint64_t)*(uint64_t *)((uint64_t *)(&(tmp_reg.x0_zero))+addr);
    // // return 0;
    // // return addr;
    // // printf("gdb_cmd_func_ext_api_cpu_read_reg %lld %llx\n",addr,tmp_reg.pc);
    // // printf("gdb_cmd_func_ext_api_cpu_read_reg tmp_val %lld %llx\n",addr,tmp_val);
    // return tmp_val;
    return juicevm_cpu_read_reg(addr);
}

volatile void gdb_cmd_func_ext_api_cpu_read_mem(volatile uint64_t addr,uint8_t *out,uint64_t len){
    // printf("gdb_cmd_func_ext_api_cpu_read_mem tmp_val %llx %lld %llx\n",addr,len,out[0]);
    // static uint8_t i = 0;
    // i++;
    // out[0] = i;
    // return;
    juicevm_cpu_read_mem(addr,out,len);
}

volatile void gdb_cmd_func_ext_api_cpu_write_mem(volatile uint64_t addr,uint8_t *in,uint64_t len){
    // printf("gdb_cmd_func_ext_api_cpu_write_mem tmp_val %llx %lld %llx\n",addr,len,in[0]);
    // return;
    juicevm_cpu_write_mem(addr,in,len);
}

void gdb_cmd_func_ext_api_cpu_write_reg(volatile uint64_t addr,volatile uint64_t val){
    // tmp_reg.pc = (uint64_t)(0x123456789abcdef1);
    // volatile uint64_t * tmp_val_p = (uint64_t *)((uint64_t *)(&(tmp_reg.x0_zero))+addr);
    // *tmp_val_p = val;
    juicevm_cpu_write_reg(addr,val);
}
void gdb_ext_api_cpu_set_cpu_pc(uint64_t addr){
    // printf("gdb_ext_api_cpu_set_cpu_pc addr %llx\n",addr);
    // tmp_reg.pc = addr;
    juicevm_cpu_set_cpu_pc(addr);
}

void gdb_ext_api_cpu_set_cpu_singlestep(uint8_t en){
    // printf("gdb_ext_api_cpu_set_cpu_singlestep en %d\n",en);
    juicevm_cpu_set_cpu_singlestep(en);
}

#endif