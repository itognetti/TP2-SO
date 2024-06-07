#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <timeDriver.h>
#include <registers.h>
#include <time.h>
#include <beeper.h>
#include "tasks.h"
#include "memoryManager.h"
#include "pipes.h"
#include "semaphore.h"
#include <types.h>
#include <childProcess.h>

#define BACKGROUND 0
#define STDIN 1
#define STDOUT 1
#define STDERR 2

enum {SYS_READ = 0, SYS_WRITE, SYS_CLEAR, SYS_TIME, SYS_HOLDER, SYS_BEEP, SYS_INFO_REG, SYS_DRAW, SYS_ALLOC, SYS_FREE, SYS_MEMORY_MAN_STATUS, 
    SYS_PRINT_MEMORY, SYS_REGISTER_PIPE, SYS_REGISTER_PIPE_AVAILABLE, SYS_READ_PIPE, SYS_WRITE_PIPE, SYS_INFO_PIPE, SYS_DESTROY_PIPE,
    SYS_SEM_REG, SYS_SEM_REG_AVAILABLE, SYS_SEM_WAIT, SYS_SEM_SIGNAL, SYS_SEM_INFO, SYS_SEM_DESTROY, SYS_CHILD_PROC_REGISTER, SYS_CHILD_WAIT,
    SYS_GET_PID, SYS_REGISTER_PROC, SYS_ALIVE_PROC, SYS_PAUSE_OR_UNPAUSE_PROC, SYS_INFO_PROC,SYS_KILL_PROC, SYS_NICE, SYS_RENOUNCE_CPU};

#define MAX_MEM_POS 0x7FFFFFFFF		
#define MIN_MEM_POS 0x400000
#define INVALID_MEM_POS -1
#define MAX_MEM_READ 16
#define BYTE_LENGTH 2

void sys_write(int fd, char * string, int row, int col, int color);
void sys_read(int fd, char *buffer, int length);
void sys_clear();
void sys_time(date currentDate);
void sys_holder(int time);
void sys_beep(uint32_t beepTime);
void sys_info_reg();
void sys_draw(int row, int col, int color);
void sys_write_to_stdout(char *buffer, int row, int col, int color);

uint64_t sys_allocate_memory(uint64_t length);
void sys_free(void * pointer);
uint64_t sys_memory_man_status(uint64_t * buffer);
unsigned int sys_print_memory(uint64_t pos, char * buffer);

uint64_t sys_create_pipe(unsigned int pipeId);
uint64_t sys_create_pipe_available();
uint64_t sys_read_from_pipe(unsigned int pipeId, char * destination, unsigned int count);
uint64_t sys_write_to_pipe(unsigned int pipeId, const char * source, unsigned int count);
uint64_t sys_get_pipe_info(pipesInfo * info);
void sys_destroy_pipe(unsigned int pipeId);

uint64_t sys_create_sem(unsigned int semaphoreId, unsigned int initialValue);
uint64_t sys_create_sem_available(unsigned int initialValue);
uint64_t sys_signal_sem(unsigned int semaphoreId);
uint64_t sys_wait_sem(unsigned int semaphoreId);
uint64_t sys_get_semaphore_info(semaphoreInfo * info);
void sys_destroy_sem(unsigned int semaphoreId);

uint64_t sys_register_child_process(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0);
void sys_wait_for_children();
uint64_t sys_get_pid();
uint64_t sys_kill_process(unsigned int processPid);
uint64_t sys_pause_or_unpause_process(unsigned int processPid);
uint64_t sys_info_process(processInfo * info);
uint64_t sys_alive_process(unsigned int processPid);
uint64_t sys_register_process(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0);
void sys_renounce_cpu();
uint64_t sys_nice(uint8_t pid, int delta);

#endif