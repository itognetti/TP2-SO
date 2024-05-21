#include "../include/syscalls.h"

void syscallDispatcher(uint64_t id, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4){
    switch(id){
        case SYS_WRITE:
            sys_write((char *) param1, (int) param2, (int) param3, (int) param4);
            break;
        case SYS_READ:
            sys_read((char *) param1, (int) param2);
            break;
        case SYS_CLEAR:
            sys_clear();
            break;
        case SYS_TIME:
            sys_time((date) param1);
            break;
        case SYS_HOLDER:
            sys_holder((int) param1);
            break;
        case SYS_BEEP:
            sys_beep((uint32_t) param1);
            break;
        case SYS_INFO_REG:
            sys_info_reg();
            break;
        case SYS_DRAW:
            sys_draw((int) param1, (int) param2, (int) param3);
            break;
    }
}