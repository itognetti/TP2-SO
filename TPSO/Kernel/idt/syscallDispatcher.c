// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/syscalls.h"

uint64_t syscallDispatcher(uint64_t id, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4){
    switch(id){
        case SYS_WRITE:
            sys_write_to_stdout((char *) param1, (int) param2, (int) param3, (int) param4);
            return 0;
        case SYS_READ:
            sys_read((int) param1, (char *) param2, (int) param3);
            return 0;
        case SYS_CLEAR:
            sys_clear();
            return 0;
        case SYS_TIME:
            sys_time((date) param1);
            return 0;
        case SYS_HOLDER:
            sys_holder((int) param1);
            return 0;
        case SYS_BEEP:
            sys_beep((uint32_t) param1);
            return 0;
        case SYS_INFO_REG:
            sys_info_reg();
            return 0;
        case SYS_DRAW:
            sys_draw((int) param1, (int) param2, (int) param3);
            return 0;
        case SYS_ALLOC:
            return sys_allocate_memory((uint64_t) param1);
        case SYS_FREE:
            sys_free((void *) param1);
            return 0;
        case SYS_MEMORY_MAN_STATUS:
            return sys_memory_man_status((uint64_t *) param1);
        case SYS_PRINT_MEMORY:
            return sys_print_memory((uint64_t) param1, (char *) param2);
        case SYS_REGISTER_PIPE:
            return sys_create_pipe((unsigned int) param1);
        case SYS_REGISTER_PIPE_AVAILABLE:
            return sys_create_pipe_available();
        case SYS_READ_PIPE:
            return sys_read_from_pipe((unsigned int) param1, (char *) param2, (unsigned int) param3);
        case SYS_WRITE_PIPE:
            return sys_write_to_pipe((unsigned int) param1, (char *) param2, (unsigned int) param3);
        case SYS_INFO_PIPE:
            return sys_get_pipe_info((pipesInfo *) param1);
        case SYS_DESTROY_PIPE:
            sys_destroy_pipe((unsigned int) param1);
            return 0;
        case SYS_SEM_REG:
            return sys_create_sem((unsigned int) param1, (unsigned int) param2);
        case SYS_SEM_REG_AVAILABLE:
            return sys_create_sem_available((unsigned int) param1);
        case SYS_SEM_WAIT:
            return sys_wait_sem((unsigned int) param1);
        case SYS_SEM_SIGNAL: 
            return sys_signal_sem((unsigned int) param1);
        case SYS_SEM_INFO:
            return sys_get_semaphore_info((semaphoreInfo *) param1);
        case SYS_SEM_DESTROY:
            sys_destroy_sem((unsigned int) param1);
            return 0;
        case SYS_CHILD_PROC_REGISTER:
            return sys_register_child_process((unsigned int) param1, (uint8_t) param2, (uint8_t) param3, (char **) param4);
        case SYS_CHILD_WAIT:
            sys_wait_for_children();
            return 0;
        case SYS_GET_PID:
            return sys_get_pid();
        case SYS_REGISTER_PROC:
            return sys_register_process((uint64_t) param1, (uint8_t) param2, (uint8_t) param3, (char**) param4);
        case SYS_ALIVE_PROC:
            return sys_alive_process((unsigned int) param1);
        case SYS_PAUSE_OR_UNPAUSE_PROC:
            return sys_pause_or_unpause_process((unsigned int) param1);
        case SYS_INFO_PROC:
            return sys_info_process((processInfo *) param1);
        case SYS_KILL_PROC:
            return sys_kill_process((unsigned int) param1);
        case SYS_NICE:
			sys_nice((uint8_t) param1, (int) param2);
            return 0;
        case SYS_RENOUNCE_CPU:
            sys_renounce_cpu();
            return 0;
    }
    return 1;
}