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

#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_CLEAR 2
#define SYS_TIME 3
#define SYS_HOLDER 4
#define SYS_BEEP 5
#define SYS_INFO_REG 6
#define SYS_DRAW 7

#define SYS_ALLOC 8
#define SYS_FREE 9

#define SYS_PIPE_INFO 10
#define SYS_READ_PIPE 11
#define SYS_REGISTER_PIPE_AVAILABLE 12
#define SYS_WRITE_PIPE 13
#define SYS_DESTROY_PIPE 14

#define SYS_SEM_SIGNAL 15
#define SYS_SEM_DESTROY 16
#define SYS_SEM_REG 17
#define SYS_SEM_REG_AVAILABLE 18
#define SYS_SEM_WAIT 19

#define SYS_CHILD_WAIT 20
#define SYS_CHILD_PROC_REGISTER 21

#define SYS_GET_PID 22

#define SYS_KILL_PROC 23
#define SYS_PAUSE_OR_UNPAUSE_PROC 24
#define SYS_REGISTER_PROC 25

#define SYS_NICE 26

#define MAX_MEM_POS 0x7FFFFFFFF		
#define MIN_MEM_POS 0x400000
#define INVALID_MEM_POS -1
#define MAX_MEM_READ 16
#define BYTE_LENGTH 2

void sys_write(int fd, char * string, int row, int col, int color);
void sys_read(int fd, char *buffer, int length);
void sys_clear();
void sys_time();
void sys_holder(int time);
void sys_beep(uint32_t beepTime);
void sys_info_reg();
void sys_draw(int row, int col, int color);
uint64_t sys_free(void * pointer);
void sys_write_to_stdout(char *buffer, int row, int col, int color);

void sys_allocate_memory(uint64_t length);
void sys_destroy_pipe(unsigned int pipeId);
void sys_get_pipe_info(pipesInfo * info);
void sys_read_from_pipe(unsigned int pipeId, char * destination, unsigned int count);
void sys_create_pipe_available();
void sys_write_to_pipe(unsigned int pipeId, const char * source, unsigned int count);
void sys_wait_sem(unsigned int semaphoreId);
void sys_signal_sem(unsigned int semaphoreId);
void sys_destroy_sem(unsigned int semaphoreId);
void sys_create_sem(unsigned int semaphoreId, unsigned int initialValue);
void sys_create_sem_available(unsigned int initialValue);
void sys_register_child_process(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0);
void sys_wait_for_children();
void sys_get_pid();
void sys_kill_process(unsigned int processPid);
void sys_pause_and_unpause_process(unsigned int processPid);
uint64_t sys_register_process(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0);
void sys_nice(uint8_t pid, int delta);

#endif