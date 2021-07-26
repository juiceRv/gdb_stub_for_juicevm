#ifndef _GDB_CFG_RECVBUF_SIZE_
#define _GDB_CFG_RECVBUF_SIZE_  1024
#endif
#include "gdb_stub.h"
#include "gdb_api.h"

typedef struct  {
    uint16_t recv_buf_index;
    uint16_t recv_buf_sum;
    uint8_t recv_buf[_GDB_CFG_RECVBUF_SIZE_];
    uint16_t recv_csum;
    gdb_rs_state_t cur_gdb_state;

}gdb_deal_state_t;
gdb_deal_state_t gdb_deal_state_global = {
    .cur_gdb_state = GDB_RS_IDLE,
};
/* return -1 if error, 0 if OK */
void gdb_ext_api_put_packet_binary(const char *buf, int len)
{
    int csum, i;
    uint8_t footer[3];
    gdb_ext_api_send_buf((const char *) "$",1);
    gdb_ext_api_send_buf((const char *) buf, len);
    csum = 0;
    for(i = 0; i < len; i++) {
        csum += buf[i];
    }
    footer[0] = '#';
    footer[1] = gdb_ext_api_tohex((csum >> 4) & 0xf);
    footer[2] = gdb_ext_api_tohex((csum) & 0xf);
    gdb_ext_api_send_buf((const char *) footer, 3);

    gdb_deal_state_global.cur_gdb_state = GDB_RS_WAIT_RSPON;
    // i = get_char();
    // if (i < 0)
    //     return -1;
    // if (i == '+')
    //     return 0;
}

/* return -1 if error, 0 if OK */
void gdb_ext_api_put_packet(const char *buf)
{
    gdb_ext_api_put_packet_binary(buf, strlen(buf));
}
static gdb_ext_api_trap_packet_str_buf[__CPU_MAX_PACKET_LENGTH_];
void gdb_ext_api_trap_packet_send(const char *buf){
    strcpy(gdb_ext_api_trap_packet_str_buf,buf);
}
uint8_t gdb_handle_trap_packet_send(void){
    // 只允许在执行c，s等指令下返回非零的值，返回非零会忽略当前接收到的数据
    // printf("gdb_handle_trap_packet_send\n");
    if(strlen(gdb_ext_api_trap_packet_str_buf) > 0){
        gdb_ext_api_put_packet(gdb_ext_api_trap_packet_str_buf);
        gdb_ext_api_trap_packet_str_buf[0] = '\0'; // clear buf
        return 1;
    }
    return 0;
}

void gdb_ext_api_deal_char(const char ch)
{
    // printf("gdb_ext_api_deal_char: %c %d\n",ch,gdb_deal_state_global.cur_gdb_state);
    uint8_t reply;
    static uint8_t had_packet_send = 0;
    // printf("gdb_ext_api_deal_char: %c %d had_packet_send:%d\n",ch,gdb_deal_state_global.cur_gdb_state,had_packet_send);
_gdb_ext_api_deal_char_check:
    if(had_packet_send == 0){
        if(gdb_handle_trap_packet_send()){
            had_packet_send = 1;
        }else{
            had_packet_send = 0;
        }
    }
    if (gdb_deal_state_global.cur_gdb_state == GDB_RS_WAIT_RSPON) {
    // When either the host or the target machine receives a packet, 
    // the first response expected is an acknowledgment: 
    // either `+' (to indicate the package was received correctly) or `-' (to request retransmission):
        gdb_deal_state_global.cur_gdb_state = GDB_RS_IDLE;
        if (ch == '-') {
            // 暂不实现重传机制
        } else if (ch == '+') {
            if(had_packet_send == 1) {
                had_packet_send = 0;
                goto _gdb_ext_api_deal_char_check;
            }else{
                return;
            }
            
        } else {
            // trace_gdbstub_io_got_unexpected(ch);
            if(had_packet_send == 1) {
                had_packet_send = 0;
                goto _gdb_ext_api_deal_char_check;
            }else{
                return;
            }
        }

        if (ch == '+' || ch == '$') {
            // 如果实现了重传机制就清空发送缓冲区
            if(had_packet_send == 1) {
                had_packet_send = 0;
                goto _gdb_ext_api_deal_char_check;
            }else{
                return;
            }
        }
        if (ch != '$'){
            if(had_packet_send == 1) {
                had_packet_send = 0;
                goto _gdb_ext_api_deal_char_check;
            }else{
                return;
            }
        }
    }
    
    if (gdb_ext_api_cpustate_is_running()) {
        // when the CPU is running, we cannot do anything except stop
        // it when receiving a char, so we stop the cpu when the cpu state is running
        gdb_ext_api_stop_cpu();
    } else
    {
        switch(gdb_deal_state_global.cur_gdb_state) {
        case GDB_RS_IDLE:
            if (ch == '$') {
                /* start of cmd packet */
                gdb_deal_state_global.recv_buf_index = 0;
                gdb_deal_state_global.recv_buf_sum = 0;
                gdb_deal_state_global.cur_gdb_state = GDB_RS_GETLINE;
            } else {
                // gdb recv err
            }
            break;
        case GDB_RS_GETLINE:
            if (ch == '}') {
                /* start escape sequence */
                gdb_deal_state_global.cur_gdb_state = GDB_RS_GETLINE_ESC;
                gdb_deal_state_global.recv_buf_sum += ch;
            } else if (ch == '*') {
                /* start run length encoding sequence */
                gdb_deal_state_global.cur_gdb_state = GDB_RS_GETLINE_RLE;
                gdb_deal_state_global.recv_buf_sum += ch;
            } else if (ch == '#') {
                /* end of command, start of checksum*/
                gdb_deal_state_global.cur_gdb_state = GDB_RS_CHKSUM1;
            } else if (gdb_deal_state_global.recv_buf_index >= sizeof(gdb_deal_state_global.recv_buf) - 1) {
                // gdbstub_err_overrun
                gdb_deal_state_global.cur_gdb_state = GDB_RS_IDLE;
            } else {
                /* unescaped command character */
                gdb_deal_state_global.recv_buf[gdb_deal_state_global.recv_buf_index++] = ch;
                gdb_deal_state_global.recv_buf_sum += ch;
            }
            break;
        case GDB_RS_GETLINE_ESC:
            if (ch == '#') {
                /* unexpected end of command in escape sequence */
                gdb_deal_state_global.cur_gdb_state = GDB_RS_CHKSUM1;
            } else if (gdb_deal_state_global.recv_buf_index >= sizeof(gdb_deal_state_global.recv_buf) - 1) {
                /* command buffer overrun */
                // gdbstub_err_overrun
                gdb_deal_state_global.cur_gdb_state = GDB_RS_IDLE;
            } else {
                /* parse escaped character and leave escape state */
                gdb_deal_state_global.recv_buf[gdb_deal_state_global.recv_buf_index++] = ch ^ 0x20;
                gdb_deal_state_global.recv_buf_sum += ch;
                gdb_deal_state_global.cur_gdb_state = GDB_RS_GETLINE;
            }
            break;
        case GDB_RS_GETLINE_RLE:
            // Response data can be run-length encoded to save space. 
            // A `*' means that the next character is an ASCII encoding giving a repeat count which stands for that many repetitions of the character preceding the `*'. 
            // The encoding is n+29, yielding a printable character where n >=3 (which is where rle starts to win). 
            // The printable characters `$', `#', `+' and `-' or with a numeric value greater than 126 should not be used.
            if (ch < ' ' || ch == '#' || ch == '$' || ch > 126) {
                /* invalid RLE count encoding */
                //  gdbstub_err_invalid_repeat;
                gdb_deal_state_global.cur_gdb_state = GDB_RS_GETLINE;
            } else {
                /* decode repeat length */
                int repeat = ch - ' ' + 3;
                if (gdb_deal_state_global.recv_buf_index + repeat >= sizeof(gdb_deal_state_global.recv_buf) - 1) {
                    /* that many repeats would overrun the command buffer */
                    // gdbstub_err_overrun
                    gdb_deal_state_global.cur_gdb_state = GDB_RS_IDLE;
                } else if (gdb_deal_state_global.recv_buf_index < 1) {
                    /* got a repeat but we have nothing to repeat */
                    // gdbstub_err_invalid_rle
                    gdb_deal_state_global.cur_gdb_state = GDB_RS_GETLINE;
                } else {
                    /* repeat the last character */
                    memset(gdb_deal_state_global.recv_buf + gdb_deal_state_global.recv_buf_index,
                           gdb_deal_state_global.recv_buf[gdb_deal_state_global.recv_buf_index - 1], repeat);
                    gdb_deal_state_global.recv_buf_index += repeat;
                    gdb_deal_state_global.recv_buf_sum += ch;
                    gdb_deal_state_global.cur_gdb_state = GDB_RS_GETLINE;
                }
            }
            break;
        case GDB_RS_CHKSUM1:
            /* get high hex digit of checksum */
            if (!gdb_ext_api_isxdigit(ch)) {
                // gdbstub_err_checksum_invalid;
                gdb_deal_state_global.cur_gdb_state = GDB_RS_GETLINE;
                break;
            }
            gdb_deal_state_global.recv_buf[gdb_deal_state_global.recv_buf_index] = '\0';
            gdb_deal_state_global.recv_csum = gdb_ext_api_fromhex(ch) << 4;
            gdb_deal_state_global.cur_gdb_state = GDB_RS_CHKSUM2;
            break;
        case GDB_RS_CHKSUM2:
            /* get low hex digit of checksum */
            if (!gdb_ext_api_isxdigit(ch)) {
                // gdbstub_err_checksum_invalid;
                gdb_deal_state_global.cur_gdb_state = GDB_RS_GETLINE;
                break;
            }
            gdb_deal_state_global.recv_csum |= gdb_ext_api_fromhex(ch);

            if (gdb_deal_state_global.recv_csum != (gdb_deal_state_global.recv_buf_sum & 0xff)) {
                // gdbstub_err_checksum_incorrect(gdbserver_state.line_sum, gdbserver_state.line_csum);
                /* send NAK reply */
                reply = '-';
                gdb_ext_api_send_buf(&reply, 1);
                gdb_deal_state_global.cur_gdb_state = GDB_RS_IDLE;
            } else {
                /* send ACK reply */
                reply = '+';
                gdb_ext_api_send_buf(&reply, 1);
                gdb_deal_state_global.cur_gdb_state = gdb_handle_packet(gdb_deal_state_global.recv_buf,gdb_deal_state_global.recv_buf_index);
            }
            break;
        default:
            abort();
        }
    }
}

void gdb_stub_ext_api_start(void){
    
    gdb_deal_state_global.cur_gdb_state = GDB_RS_IDLE;
    gdb_deal_state_global.recv_buf_index = 0;
    gdb_deal_state_global.recv_buf_sum = 0;
    gdb_deal_state_global.recv_csum = 0;

}

void gdb_stub_ext_api_stop(void){


}