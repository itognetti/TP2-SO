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
        case SYS_ALLOC:
            sys_allocate_memory((uint64_t) param1);
            break;
        case SYS_DESTROY_PIPE:
            sys_destroy_pipe((unsigned int) param1);
            break;
        case SYS_PIPE_INFO:
            sys_get_pipe_info((pipesInfo *) param1);
            break;
        case SYS_READ_PIPE:
            sys_read_from_pipe((unsigned int) param1, (char *) param2, (unsigned int) param3);
            break;
        case SYS_REGISTER_PIPE_AVAILABLE:
            sys_create_pipe_available();
            break;
        case SYS_WRITE_PIPE:
            sys_write_to_pipe((unsigned int) param1, (char *) param2, (unsigned int) param3);
            break;
        case SYS_SEM_WAIT:
            sys_wait_sem((unsigned int) param1);
            break;
        case SYS_SEM_SIGNAL: 
            sys_signal_sem((unsigned int) param1);
            break;
        case SYS_SEM_DESTROY:
            sys_destroy_sem((unsigned int) param1);
            break;
        case SYS_SEM_REG:
            sys_create_sem((unsigned int) param1, (unsigned int) param2);
            break;
        case SYS_SEM_REG_AVAILABLE:
            sys_create_sem_available((unsigned int) param1);
            break;
        case SYS_CHILD_PROC_REGISTER:
            sys_register_child_process((uint64_t) param1, (uint8_t) param2, (uint8_t) param3, (char **) param4);
            break;
        case SYS_CHILD_WAIT:
            sys_wait_for_children();
            break;
        case SYS_GET_PID:
            sys_get_pid();
            break;
    }
}