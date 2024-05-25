#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <timeDriver.h>
#include <registers.h>
#include <time.h>
#include <beeper.h>
#include <tasks.h>
#include <memoryManager.h>
#include <pipes.h>
#include <semaphore.h>

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
#define SYS_DESTROY_PIPE 9
#define SYS_PIPE_INFO 10
#define SYS_READ_PIPE 11
#define SYS_REGISTER_PIPE_AVAILABLE 12
#define SYS_WRITE_PIPE 13

#define SYS_SEM_WAIT 14
#define SYS_SEM_SIGNAL 15
#define SYS_SEM_DESTROY 16
#define SYS_SEM_REG 17
#define SYS_SEM_REG_AVAILABLE 18

#define SYS_CHILD_PROC_REGISTER 19
#define SYS_CHILD_WAIT 20

#define SYS_GET_PID 21

void sys_write(char * string, int row, int col, int color);
void sys_read(char *buffer, int length);
void sys_clear();
void sys_time();
void sys_holder(int time);
void sys_beep(uint32_t beepTime);
void sys_info_reg();
void sys_draw(int row, int col, int color);

#endif