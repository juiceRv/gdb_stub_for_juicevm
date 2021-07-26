#include "gdb_stub.h"
#include "gdb_api.h"
#include "gdb_break_and_watch_point.h"
// enum {
//     GDB_SIGNAL_0 = 0,
//     GDB_SIGNAL_INT = 2,
//     GDB_SIGNAL_QUIT = 3,
//     GDB_SIGNAL_TRAP = 5,
//     GDB_SIGNAL_ABRT = 6,
//     GDB_SIGNAL_ALRM = 14,
//     GDB_SIGNAL_IO = 23,
//     GDB_SIGNAL_XCPU = 24,
//     GDB_SIGNAL_UNKNOWN = 143
// };

static const char hex_asc[] = "0123456789abcdef";

static inline char hex_asc_lo(unsigned char x)
{
	return hex_asc[((x) & 0x0f)];
}

static inline char hex_asc_hi(unsigned char x)
{
	return hex_asc[((x) & 0xf0) >> 4];
}

static int hex_to_bin(char ch)
{
	if((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	if((ch >= '0') && (ch <= '9'))
		return ch - '0';
	if((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	return 0;
}

static void mem_to_hex(char * buf, unsigned char * mem, int len)
{
	char * p = buf;
	int i;

	for(i = 0; i < len; i++)
	{
		*p++ = hex_asc_hi(mem[i]);
		*p++ = hex_asc_lo(mem[i]);
	}
	*p = 0;
}

static void hex_to_mem(unsigned char * mem, char * buf, int len)
{
	int i;

	for(i = 0; i < len; i++)
	{
		mem[i] = (hex_to_bin(buf[0]) << 4) | hex_to_bin(buf[1]);
		buf += 2;
	}
}

void gdb_handle_halt(const char * buf,const uint16_t len){
    // thread:p cpu_pid.cpu_index
    // thread:p%02x.%02x 
    unsigned char tmp_str_buf[19];
    sprintf(tmp_str_buf,"T%02xthread:%02x;",GDB_SIGNAL_TRAP,0);
    // printf("gdb_handle_halt %d:%s\n",strlen(tmp_str_buf),tmp_str_buf);
    gdb_ext_api_put_packet(tmp_str_buf);
    /*
     * Remove all the breakpoints when this query is issued,
     * because gdb is doing an initial connect and the state
     * should be cleaned up.
     */
    // gdb_breakpoint_remove_all();
}

void gdb_handle_set_baud(const char * buf,const uint16_t len){
    /* Default invalid command */
    gdb_ext_api_put_packet("");
}

void gdb_ext_api_step_handle(void){
    // loop check breakpoint

}
void gdb_handle_set_breakpoint(const char * buf,const uint16_t len){
    #if ( defined(__CPU_ADDR_XLEN_) && __CPU_ADDR_XLEN_ == 8)
    uint64_t tmp_addr = 0;
    tmp_addr |= gdb_ext_api_fromhex(buf[1 ]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[2 ]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[3 ]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[4 ]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[5 ]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[6 ]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[7 ]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[8 ]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[9 ]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[10]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[11]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[12]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[13]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[14]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[15]);tmp_addr <<= 4;
    tmp_addr |= gdb_ext_api_fromhex(buf[16]);
    // printf("gdb_handle_set_breakpoint addr %llx\n",tmp_addr);
    // if(){

    // }
    gdb_ext_api_put_packet("");
    #endif
}



void gdb_handle_continue(const char * buf,const uint16_t len){
    #if ( defined(__CPU_ADDR_XLEN_) && __CPU_ADDR_XLEN_ == 8)
    uint64_t tmp_addr = 0;
    if (len == (1+16)) {
        tmp_addr |= gdb_ext_api_fromhex(buf[1 ]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[2 ]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[3 ]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[4 ]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[5 ]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[6 ]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[7 ]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[8 ]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[9 ]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[10]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[11]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[12]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[13]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[14]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[15]);tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[16]);
        gdb_ext_api_cpu_set_cpu_pc(tmp_addr);
    }
    #endif
    gdb_ext_api_cpu_set_cpu_singlestep(0);
    gdb_ext_api_start_cpu();
    
    // gdb_break_and_watch_point_check(gdb_check_t check_type,volatile uint64_t pc,volatile uint64_t mem_addr,gdb_mem_optype_t mem_optype,volatile uint64_t len)
    // test
    // gdb_ext_api_trap_packet_send("T05rwatch:0123456789abcdef;");
    // gdb_ext_api_trap_packet_send("T05watch:123456;");
    // // gdb_ext_api_trap_packet_send("T05hwbreak:;");
    // gdb_ext_api_deal_char("");
}

void gdb_handle_detach(const char * buf,const uint16_t len){

    if (len < 1) {
        gdb_ext_api_put_packet("E22");
        return;
    }

    // gdb_process_breakpoint_remove_all(process);
    gdb_ext_api_stub_exit();
    // gdb_ext_api_put_packet("OK");
}

void gdb_handle_read_all_registers(const char * buf,const uint16_t len){
    #if ( defined(__CPU_ADDR_XLEN_) && __CPU_ADDR_XLEN_ == 8 )
    uint8_t tmp_regs_str_buf[__CPU_REGS_NUM_ * __CPU_ADDR_XLEN_*2+1];
    volatile uint64_t tmp_regs_val;
    uint16_t tmp_off = 0;
    memset(tmp_regs_str_buf,0,sizeof(tmp_regs_str_buf));
    for(uint64_t addr_i = 0;addr_i < __CPU_REGS_NUM_; addr_i++ ){
        tmp_regs_val = (volatile uint64_t)(gdb_cmd_func_ext_api_cpu_read_reg(addr_i));
        // printf("reg %lld:%llx\n",addr_i,gdb_cmd_func_ext_api_cpu_read_reg(addr_i));

        // tmp_off += sprintf(tmp_regs_str_buf+tmp_off,"%02x",tmp_regs_val>>(8*7));
        // tmp_off += sprintf(tmp_regs_str_buf+tmp_off,"%02x",tmp_regs_val>>(8*6));
        // tmp_off += sprintf(tmp_regs_str_buf+tmp_off,"%02x",tmp_regs_val>>(8*5));
        // tmp_off += sprintf(tmp_regs_str_buf+tmp_off,"%02x",tmp_regs_val>>(8*4));
        // tmp_off += sprintf(tmp_regs_str_buf+tmp_off,"%02x",tmp_regs_val>>(8*3));
        // tmp_off += sprintf(tmp_regs_str_buf+tmp_off,"%02x",tmp_regs_val>>(8*2));
        // tmp_off += sprintf(tmp_regs_str_buf+tmp_off,"%02x",tmp_regs_val>>(8*1));
        // tmp_off += sprintf(tmp_regs_str_buf+tmp_off,"%02x",tmp_regs_val>>(8*0));
        
        // printf("reg %lld:%d\n",addr_i,tmp_off);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)]    = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*1))&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+1]  = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*0))&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+2]  = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*3))&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+3]  = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*2))&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+4]  = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*5))&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+5]  = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*4))&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+6]  = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*7) )&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+7]  = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*6) )&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+8]  = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*9) )&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+9]  = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*8) )&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+10] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*11) )&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+11] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*10) )&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+12] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*13) )&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+13] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*12) )&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+14] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*15) )&0xf);
        tmp_regs_str_buf[(addr_i * __CPU_ADDR_XLEN_*2)+15] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*14) )&0xf);
    }
    tmp_regs_str_buf[sizeof(tmp_regs_str_buf)] = '\0';
    // printf("reg %d:%s\n",strlen(tmp_regs_str_buf),tmp_regs_str_buf);
    
    gdb_ext_api_put_packet(tmp_regs_str_buf);
    #endif
    
}
void gdb_handle_write_all_registers(const char * buf,const uint16_t len){
    #if ( defined(__CPU_ADDR_XLEN_) && __CPU_ADDR_XLEN_ == 8 )
    volatile uint64_t tmp_regs_val;
    volatile uint16_t tmp_len;tmp_len = len-1;
    char * tmp_buf_p;tmp_buf_p = buf+1;
    for(uint64_t addr_i = 0; addr_i < __CPU_REGS_NUM_; addr_i++){
        tmp_regs_val  = (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*1 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*0 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*3 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*2 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*5 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*4 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*7 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*6 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*9 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*8 ));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*11));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*10));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*13));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*12));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*15));
        tmp_regs_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex((uint8_t)(*(tmp_buf_p++)))) << (4*14));
        // printf("write reg %lld:%llx %d\n",addr_i,tmp_regs_val,tmp_len);
        gdb_cmd_func_ext_api_cpu_write_reg(addr_i,tmp_regs_val);
        
        // printf("reg %lld:%d\n",addr_i,tmp_off);
        tmp_len -= 8*2;
    }
    if(*(tmp_buf_p++) != '\0'){
        // err
        return;
    }
    gdb_ext_api_put_packet("OK");
    #endif
}

#define GDB_ATTACHED "1"

void gdb_handle_general_query(const char * buf,const uint16_t len){
    unsigned char tmp_str_buf[100];
    uint8_t tmp_offset = 0;
    // printf("gdb_handle_general_query %s\n",buf);
    if(strncmp(&(buf[1]),"Supported",sizeof("Supported")-1) == 0){
        tmp_offset += sprintf(tmp_str_buf,"PacketSize=%x", __CPU_MAX_PACKET_LENGTH_);
        tmp_offset += sprintf(tmp_str_buf+tmp_offset,"%s",";ReverseStep+;ReverseContinue+");
        gdb_ext_api_put_packet(tmp_str_buf);
        return;
    }
    if(strncmp(&(buf[1]),"Attached",sizeof("Attached")-1) == 0){
        gdb_ext_api_put_packet(GDB_ATTACHED);
        return;
    }
    gdb_ext_api_put_packet("");
}

void gdb_handle_set_thread(const char * buf,const uint16_t len){
    // printf("gdb_handle_set_thread %s\n",buf);
    if(strncmp(&(buf[1]),"C",sizeof("C")-1) == 0){
        gdb_ext_api_put_packet("OK");
        return;
    }
    gdb_ext_api_put_packet("");
}

void gdb_handle_read_memory(const char * buf,const uint16_t len){

    #if ( defined(__CPU_ADDR_XLEN_) && __CPU_ADDR_XLEN_ == 8 )
    char tmp_mem_buf[__CPU_MAX_PACKET_LENGTH_/2];
    char tmp_mem_hexstr_buf[__CPU_MAX_PACKET_LENGTH_];
    volatile uint64_t tmp_offset;tmp_offset = 0;
    volatile uint64_t tmp_addr;tmp_addr = 0;
    volatile uint64_t tmp_len;tmp_len = 0;

    // if(buf[9] == ','){
    //     gdb_ext_api_put_packet("E22");
    //     return;
    // }

    tmp_offset = 1;
    tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != ','){
        tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    tmp_offset++;
    
    tmp_len |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != '\0'){
        tmp_len <<= 4;
        tmp_len |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    // printf("gdb_handle_read_memory %llx %d\n",tmp_addr,tmp_len);
    if(tmp_len > (__CPU_MAX_PACKET_LENGTH_ / 2))
    {
        gdb_ext_api_put_packet("E22");
        return;
    }

    gdb_cmd_func_ext_api_cpu_read_mem(tmp_addr,tmp_mem_buf,tmp_len);
    mem_to_hex(tmp_mem_hexstr_buf, (unsigned char *)tmp_mem_buf, tmp_len);
    tmp_mem_hexstr_buf[tmp_len * 2] = '\0';
    gdb_ext_api_put_packet(tmp_mem_hexstr_buf);
    #endif
}

void gdb_handle_write_memory(const char * buf,const uint16_t len){

    #if ( defined(__CPU_ADDR_XLEN_) && __CPU_ADDR_XLEN_ == 8 )
    char tmp_mem_buf[__CPU_MAX_PACKET_LENGTH_/2];
    char tmp_mem_hexstr_buf[__CPU_MAX_PACKET_LENGTH_];
    volatile uint64_t tmp_offset;tmp_offset = 0;
    volatile uint64_t tmp_addr;tmp_addr = 0;
    volatile uint64_t tmp_len;tmp_len = 0;

    // if(buf[9] == ','){
    //     gdb_ext_api_put_packet("E22");
    //     return;
    // }

    tmp_offset = 1;
    tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != ','){
        tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    tmp_offset++;
    
    tmp_len |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != ':'){
        tmp_len <<= 4;
        tmp_len |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    tmp_offset++;

    // printf("gdb_handle_write_memory %llx %d\n",tmp_addr,tmp_len);
    if(tmp_len > (__CPU_MAX_PACKET_LENGTH_ / 2))
    {
        gdb_ext_api_put_packet("E22");
        return;
    }

    hex_to_mem(tmp_mem_buf, &(buf[tmp_offset]), tmp_len);
    gdb_cmd_func_ext_api_cpu_write_mem(tmp_addr,tmp_mem_buf,tmp_len);
    gdb_ext_api_put_packet("OK");
    #endif
}

void gdb_handle_write_reg(const char * buf,const uint16_t len){
    // write reg	Pn...=r...
    volatile uint64_t tmp_offset;tmp_offset = 0;
    volatile uint64_t tmp_addr;tmp_addr = 0;
    volatile uint64_t tmp_val;tmp_val = 0;

    // if(buf[9] == ','){
    //     gdb_ext_api_put_packet("E22");
    //     return;
    // }

    tmp_offset = 1;
    tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != '='){
        tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    tmp_offset++;
    
    tmp_val  = (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*1 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*0 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*3 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*2 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*5 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*4 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*7 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*6 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*9 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*8 ));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*11));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*10));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*13));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*12));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*15));
    tmp_val |= (volatile uint64_t)((uint64_t)(gdb_ext_api_fromhex(buf[tmp_offset++])) << (4*14));
    gdb_cmd_func_ext_api_cpu_write_reg(tmp_addr,tmp_val);
    gdb_ext_api_put_packet("OK");
}

void gdb_handle_read_reg(const char * buf,const uint16_t len){
    // read reg (reserved)	pn...	See write register.
    // return r....	The hex encoded value of the register in target byte order.
    uint8_t tmp_regs_str_buf[__CPU_ADDR_XLEN_*2+1];
    volatile uint64_t tmp_offset;tmp_offset = 0;
    volatile uint64_t tmp_addr;tmp_addr = 0;
    volatile uint64_t tmp_regs_val;
    // if(buf[9] == ','){
    //     gdb_ext_api_put_packet("E22");
    //     return;
    // }

    tmp_offset = 1;
    tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != '\0'){
        tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    
    tmp_regs_val = (volatile uint64_t)(gdb_cmd_func_ext_api_cpu_read_reg(tmp_addr));
    tmp_regs_str_buf[0 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*1 ))&0xf);
    tmp_regs_str_buf[1 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*0 ))&0xf);
    tmp_regs_str_buf[2 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*3 ))&0xf);
    tmp_regs_str_buf[3 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*2 ))&0xf);
    tmp_regs_str_buf[4 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*5 ))&0xf);
    tmp_regs_str_buf[5 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*4 ))&0xf);
    tmp_regs_str_buf[6 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*7 ))&0xf);
    tmp_regs_str_buf[7 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*6 ))&0xf);
    tmp_regs_str_buf[8 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*9 ))&0xf);
    tmp_regs_str_buf[9 ] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*8 ))&0xf);
    tmp_regs_str_buf[10] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*11))&0xf);
    tmp_regs_str_buf[11] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*10))&0xf);
    tmp_regs_str_buf[12] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*13))&0xf);
    tmp_regs_str_buf[13] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*12))&0xf);
    tmp_regs_str_buf[14] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*15))&0xf);
    tmp_regs_str_buf[15] = gdb_ext_api_tohex((uint64_t)((uint64_t)(tmp_regs_val)>>(uint64_t)(4*14))&0xf);
    tmp_regs_str_buf[sizeof(tmp_regs_str_buf)] = '\0';
    gdb_ext_api_put_packet(tmp_regs_str_buf);
}

void gdb_handle_insert_break_or_watchpoint_Z(const char * buf,const uint16_t len){
    // insert break or watchpoint (draft)	
    // Zt,addr,length	
    // t is type: `0' - software breakpoint, `1' - hardware breakpoint, `2' - write watchpoint, `3' - read watchpoint, `4' - access watchpoint; 
    // addr is address; length is in bytes. For a software breakpoint, length specifies the size of the instruction to be patched. 
    // For hardware breakpoints and watchpoints length specifies the memory region to be monitored. 
    // To avoid potential problems with duplicate packets, the operations should be implemented in an idempotent way.
    // reply ENN	for an error
    // reply OK	for success
    // `'	If not supported.
    volatile uint64_t tmp_offset;tmp_offset = 0;
    volatile uint8_t tmp_type;tmp_type = 0;
    volatile uint64_t tmp_addr;
    volatile uint64_t tmp_len;
    // if(buf[9] == ','){
    //     gdb_ext_api_put_packet("E22");
    //     return;
    // }

    tmp_offset = 1;
    tmp_type = gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != ',' && buf[tmp_offset] != '\0'){
        tmp_type <<= 4;
        tmp_type |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    tmp_offset++;
    tmp_addr = gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != ',' && buf[tmp_offset] != '\0'){
        tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    tmp_offset++;
    tmp_len = gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != '\0'){
        tmp_len <<= 4;
        tmp_len |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    // printf("gdb_handle_insert_break_or_watchpoint_Z type:%d addr:%llx len:%llx\n",tmp_type,tmp_addr,tmp_len);
    #define insert_break_or_watchpoint_software_breakpoint    0
    #define insert_break_or_watchpoint_hardware_breakpoint    1
    #define insert_break_or_watchpoint_write_watchpoint       2
    #define insert_break_or_watchpoint_read_watchpoint        3
    #define insert_break_or_watchpoint_access_watchpoint      4
    switch(tmp_type){
        // type
        case insert_break_or_watchpoint_software_breakpoint:
        case insert_break_or_watchpoint_hardware_breakpoint:
            if(!gdb_breakpoint_add(tmp_type,tmp_addr,tmp_len)){
                // gdb_ext_api_put_packet("OK");
                // return err
                return;
            }
        break;
        case insert_break_or_watchpoint_write_watchpoint:
        case insert_break_or_watchpoint_read_watchpoint:
        case insert_break_or_watchpoint_access_watchpoint:
            if(!gdb_watchpoint_add(tmp_type,tmp_addr,tmp_len)){
                // gdb_ext_api_put_packet("OK");
                // return err
                return;
            }
        break;
    }
    gdb_ext_api_put_packet("OK");
}

void gdb_handle_remove_break_or_watchpoint_z(const char * buf,const uint16_t len){

    volatile uint64_t tmp_offset;tmp_offset = 0;
    volatile uint8_t tmp_type;tmp_type = 0;
    volatile uint64_t tmp_addr;
    volatile uint64_t tmp_len;
    // if(buf[9] == ','){
    //     gdb_ext_api_put_packet("E22");
    //     return;
    // }

    tmp_offset = 1;
    tmp_type = gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != ',' && buf[tmp_offset] != '\0'){
        tmp_type <<= 4;
        tmp_type |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    tmp_offset++;
    tmp_addr = gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != ',' && buf[tmp_offset] != '\0'){
        tmp_addr <<= 4;
        tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    tmp_offset++;
    tmp_len = gdb_ext_api_fromhex(buf[tmp_offset++]);
    while(buf[tmp_offset] != '\0'){
        tmp_len <<= 4;
        tmp_len |= gdb_ext_api_fromhex(buf[tmp_offset++]);
    }
    // printf("gdb_handle_remove_break_or_watchpoint_z type:%d addr:%llx len:%llx\n",tmp_type,tmp_addr,tmp_len);

    switch(tmp_type){
        // type
        case insert_break_or_watchpoint_software_breakpoint:
        case insert_break_or_watchpoint_hardware_breakpoint:
            gdb_breakpoint_del(tmp_type,tmp_addr,tmp_len);
        break;
        case insert_break_or_watchpoint_write_watchpoint:
        case insert_break_or_watchpoint_read_watchpoint:
        case insert_break_or_watchpoint_access_watchpoint:
            gdb_watchpoint_del(tmp_type,tmp_addr,tmp_len);
        break;
    }
    gdb_ext_api_put_packet("OK");
}

void gdb_handle_step_s(const char * buf,const uint16_t len){
    // step	saddr	addr is address to resume. If addr is omitted, resume at same address.
    // reply	see below
    volatile uint64_t tmp_offset;tmp_offset = 0;
    volatile uint64_t tmp_addr;tmp_addr = 0;
    if(len > 1){
        tmp_offset = 1;
        tmp_addr = gdb_ext_api_fromhex(buf[tmp_offset++]);
        while(buf[tmp_offset] != '\0'){
            tmp_addr <<= 4;
            tmp_addr |= gdb_ext_api_fromhex(buf[tmp_offset++]);
        }
    }
    gdb_ext_api_cpu_set_cpu_pc(tmp_addr);
    gdb_ext_api_cpu_set_cpu_singlestep(1);
    gdb_ext_api_start_cpu();
    
    // gdb_break_and_watch_point_check(gdb_check_t check_type,volatile uint64_t pc,volatile uint64_t mem_addr,gdb_mem_optype_t mem_optype,volatile uint64_t len)
    // test
    // gdb_ext_api_trap_packet_send("T05rwatch:0123456789abcdef;");
    // // gdb_ext_api_trap_packet_send("T05hwbreak:;");
    // gdb_ext_api_deal_char("");
    // gdb_ext_api_stop_cpu();
}
int gdb_handle_packet(const char *buf,const uint16_t len)
{
    // printf("gdb_handle_packet %s\n",buf);
    if(len == 0){
        
    }
    switch (buf[0]) {
    case '!':
        // extended mode
        // Enable extended mode. In extended mode, the remote server is made persistent. The `R' packet is used to restart the program being debugged.
        // The remote target both supports and has enabled extended mode.
        gdb_ext_api_put_packet("OK");
        break;
    case '?':
        {
            // last signal
            // Indicate the reason the target halted. The reply is the same as for step and continue.
            gdb_handle_halt(buf,len);
        }
        break;
    case 'a':
        // Aarglen,argnum,arg,...
        // Reserved for future use
        break;
    case 'A':
        // reserved
        // 	Initialized `argv[]' array passed into program. arglen specifies the number of bytes in the hex encoded byte stream arg. See `gdbserver' for more details.
        break;
    case 'b':
        {
            // set baud (deprecated)
            // Change the serial line speed to baud. JTC: When does the transport layer state change? When it's received, or after the ACK is transmitted. 
            // In either case, there are problems if the command or the acknowledgment packet is dropped. 
            // Stan: If people really wanted to add something like this, and get it working for the first time, they ought to modify ser-unix.c to send some kind of out-of-band message to a specially-setup stub and have the switch happen "in between" packets, so that from remote protocol's point of view, nothing actually happened.
            // bbaud
            gdb_handle_set_baud(buf,len);
        }
        break;
    case 'B':
        {
            // set breakpoint (deprecated)
            // Baddr,mode	
            // Set (mode is `S') or clear (mode is `C') a breakpoint at addr. This has been replaced by the `Z' and `z' packets.
            gdb_handle_set_breakpoint(buf,len);
        }
        break;

    case 'c':
        {
            // continue	
            // caddr	
            // addr is address to resume. If addr is omitted, resume at current address.

            gdb_handle_continue(buf,len);
        }
        break;
    case 'C':
        {
            // continue with signal	
            // Csig;addr	
            // Continue with signal sig (hex signal number). If ;addr is omitted, resume at same address.

            gdb_ext_api_put_packet(""); // unsupport
        }
        break;
    case 'd':
            // toggle debug (deprecated)
            // d	toggle 
            // debug flag.
        break;
    case 'D':
        {
            // detach	
            // D	
            // Detach GDB from the remote system. Sent to the remote target before GDB disconnects.
            // reply no response	GDB does not check for any response after sending this packet.
            gdb_handle_detach(buf,len);

        }
        break;
    case 'e':
        //     reserved	e	Reserved for future use
        break;
    case 'E':
        // reserved	E	Reserved for future use
        break;
    case 'f':
        // reserved	f	Reserved for future use
        break;
    case 'F':
        // reserved	F	Reserved for future use
        break;
    case 'g':
        {
            // read registers	
            // g	
            // Read general registers.
            // reply XX...	Each byte of register data is described by two hex digits. 
            // The bytes with the register are transmitted in target byte order. 
            // The size of each register and their position within the `g' packet are determined by the GDB internal macros REGISTER_RAW_SIZE and REGISTER_NAME macros. 
            // The specification of several standard g packets is specified below.

            // ENN	for an error.
            gdb_handle_read_all_registers(buf,len);
        }
        break;
    case 'G':
        {
            // write regs	
            // GXX...	
            // See `g' for a description of the XX... data.
            // reply OK	for success
            // reply ENN	for an error
            gdb_handle_write_all_registers(buf,len);
        }
        break;
        // reply OK	for success
        // reply ENN	for an error
        // reserved	h	Reserved for future use
    case 'H':
        {
            // set thread	Hct...	Set thread for subsequent operations (`m', `M', `g', `G', et.al.). c = `c' for thread used in step and continue; t... can be -1 for all threads. c = `g' for thread used in other operations. If zero, pick a thread, any thread.
            gdb_handle_set_thread(buf,len);
        }
        break;
        // reply OK	for success
        // reply ENN	for an error
        // cycle step (draft)	iaddr,nnn	Step the remote target by a single clock cycle. If ,nnn is present, cycle step nnn cycles. If addr is present, cycle step starting at that address.
        // signal then cycle step (reserved)	I	See `i' and `S' for likely syntax and semantics.
        // reserved	j	Reserved for future use
        // reserved	J	Reserved for future use
        // kill request	k	FIXME: There is no description of how operate when a specific thread context has been selected (ie. does 'k' kill only that thread?).
        // reserved	l	Reserved for future use
        // reserved	L	Reserved for future use
    case 'm':
        // read memory	maddr,length	Read length bytes of memory starting at address addr. Neither GDB nor the stub assume that sized memory transfers are assumed using word alligned accesses. FIXME: A word aligned memory transfer mechanism is needed.
        // reply XX...	XX... is mem contents. Can be fewer bytes than requested if able to read only part of the data. Neither GDB nor the stub assume that sized memory transfers are assumed using word alligned accesses. FIXME: A word aligned memory transfer mechanism is needed.
        // reply ENN	NN is errno
        gdb_handle_read_memory(buf,len);
        break;
    case 'M':
        // write mem	Maddr,length:XX...	Write length bytes of memory starting at address addr. XX... is the data.
        gdb_handle_write_memory(buf,len);
        break;
        // reply OK	for success
        // reply ENN	for an error (this includes the case where only part of the data was written).
        // reserved	n	Reserved for future use
        // reserved	N	Reserved for future use
        // reserved	o	Reserved for future use
        // reserved	O	Reserved for future use
    case 'p':
        // read reg (reserved)	pn...	See write register.
        // return r....	The hex encoded value of the register in target byte order.
        gdb_handle_read_reg(buf,len);
        break;
    case 'P':
        // write reg	Pn...=r...	Write register n... with value r..., which contains two hex digits for each byte in the register (target byte order).
        // reply OK	for success
        // reply ENN	for an error
        gdb_handle_write_reg(buf,len);
        break;
    case 'q':
        {
            // general query	qquery	Request info about query. In general GDB queries have a leading upper case letter. Custom vendor queries should use a company prefix (in lower case) ex: `qfsf.var'. query may optionally be followed by a `,' or `;' separated list. Stubs must ensure that they match the full query name.
            // reply XX...	Hex encoded data from query. The reply can not be empty.
            // reply ENN	error reply
            // reply `'	Indicating an unrecognized query.
            gdb_handle_general_query(buf,len);
        }
        break;
            // general set	Qvar=val	Set value of var to val. See `q' for a discussing of naming conventions.
            // reset (deprecated)	r	Reset the entire system.
            // remote restart	RXX	Restart the program being debugged. XX, while needed, is ignored. This packet is only available in extended mode.
            // no reply	The `R' packet has no reply.
    case 's':
            // step	saddr	addr is address to resume. If addr is omitted, resume at same address.
            // reply	see below
        gdb_handle_step_s(buf,len);
    break;
            // step with signal	Ssig;addr	Like `C' but step not continue.
            // reply	see below
            // search	taddr:PP,MM	Search backwards starting at address addr for a match with pattern PP and mask MM. PP and MM are 4 bytes. addr must be at least 3 digits.
            // thread alive	TXX	Find out if the thread XX is alive.
            // reply OK	thread is still alive
            // reply ENN	thread is dead
            // reserved	u	Reserved for future use
            // reserved	U	Reserved for future use
            // reserved	v	Reserved for future use
            // reserved	V	Reserved for future use
            // reserved	w	Reserved for future use
            // reserved	W	Reserved for future use
            // reserved	x	Reserved for future use
            // write mem (binary)	Xaddr,length:XX...	addr is address, length is number of bytes, XX... is binary data. The characters $, #, and 0x7d are escaped using 0x7d.
            // reply OK	for success
            // reply ENN	for an error
            // reserved	y	Reserved for future use
            // reserved	Y	Reserved for future use
        case 'z':
            // remove break or watchpoint (draft)	zt,addr,length	See `Z'.
            gdb_handle_remove_break_or_watchpoint_z(buf,len);
            break;
        case 'Z':
            gdb_handle_insert_break_or_watchpoint_Z(buf,len);
            // insert break or watchpoint (draft)	Zt,addr,length	t is type: `0' - software breakpoint, `1' - hardware breakpoint, `2' - write watchpoint, `3' - read watchpoint, `4' - access watchpoint; addr is address; length is in bytes. For a software breakpoint, length specifies the size of the instruction to be patched. For hardware breakpoints and watchpoints length specifies the memory region to be monitored. To avoid potential problems with duplicate packets, the operations should be implemented in an idempotent way.
            // reply ENN	for an error
            // reply OK	for success
            // `'	If not supported.
            break;
            // reserved	<other>	Reserved for future use
    default:
        /* put empty packet */
        gdb_ext_api_put_packet("");
        break;
    }

    // if (cmd_parser) {
    //     run_cmd_parser(line_buf, cmd_parser);
    // }

    return GDB_RS_IDLE;
}