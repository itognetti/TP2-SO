#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <date.h>
#include "../include/types.h"
#include <stddef.h>

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

extern uint64_t _syscall(uint64_t syscall, ...);

void write(char *string, int row, int col, int color);
void read(int fd, char *buffer, int length);
void clearScreen();
void getTime(date currentDate);
void hold(int time);
void beep(uint32_t beepTime);
void getRegisters();
void putPixel(int row, int col, int color);

uint64_t alloc(int len);
uint64_t freeMem(void * ptr);

uint64_t readPipe(int pipeId, char * dest, int count);
uint64_t writePipe(int pipeId, const char * src, int count);
uint64_t pipeInfo(pipesInfo * info);
void destroyPipe( int pipeId);
uint64_t registerPipeAvailable();
uint64_t signalSemaphore(uint64_t semId);
uint64_t destroySemaphore(uint64_t semId);
uint64_t waitSemaphore(uint64_t semId);
uint64_t registerSemaphore(uint64_t semId, unsigned int val);
uint64_t registerSemaphoreAvailable(unsigned int val);
uint64_t getPid();
uint64_t waitChildren();
uint64_t registerChildProcess(uint64_t entryP, uint8_t input, uint8_t output, uint64_t arg0);
uint64_t killProcess(unsigned int processPid);
uint64_t pauseOrUnpauseProcess(unsigned int processPid);
uint64_t registerProcess(uint64_t entrypoint, uint8_t input, uint8_t output, uint64_t arg0);
uint64_t niceProcess(uint8_t pid, int delta);

#endif