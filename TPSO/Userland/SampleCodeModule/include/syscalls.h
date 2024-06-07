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

enum {SYS_READ = 0, SYS_WRITE, SYS_CLEAR, SYS_TIME, SYS_HOLDER, SYS_BEEP, SYS_INFO_REG, SYS_DRAW, SYS_ALLOC, SYS_FREE, SYS_MEMORY_MAN_STATUS, 
    SYS_PRINT_MEMORY, SYS_REGISTER_PIPE, SYS_REGISTER_PIPE_AVAILABLE, SYS_READ_PIPE, SYS_WRITE_PIPE, SYS_INFO_PIPE, SYS_DESTROY_PIPE,
    SYS_SEM_REG, SYS_SEM_REG_AVAILABLE, SYS_SEM_WAIT, SYS_SEM_SIGNAL, SYS_SEM_INFO, SYS_SEM_DESTROY, SYS_CHILD_PROC_REGISTER, SYS_CHILD_WAIT,
    SYS_GET_PID, SYS_REGISTER_PROC, SYS_ALIVE_PROC, SYS_PAUSE_OR_UNPAUSE_PROC, SYS_INFO_PROC,SYS_KILL_PROC, SYS_NICE, SYS_RENOUNCE_CPU};

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
uint64_t memoryManStatus(uint64_t * buffer);
uint64_t printMem(uint64_t pos, char * buffer);

uint64_t registerPipe(unsigned int pipeId);
uint64_t registerPipeAvailable();
uint64_t readPipe(int pipeId, char * dest, int count);
uint64_t writePipe(int pipeId, const char * src, int count);
uint64_t pipeInfo(pipesInfo * info);
void destroyPipe( int pipeId);

uint64_t registerSemaphore(uint64_t semId, unsigned int val);
uint64_t registerSemaphoreAvailable(unsigned int val);
uint64_t signalSemaphore(uint64_t semId);
uint64_t waitSemaphore(uint64_t semId);
void destroySemaphore(uint64_t semId);

uint64_t registerChildProcess(uint64_t entryP, uint8_t input, uint8_t output, char ** arg0);
void waitChildren();
uint64_t getPid();
uint64_t registerProcess(uint64_t entrypoint, uint8_t input, uint8_t output, char ** arg0);
uint64_t aliveProcess(unsigned int processPid);
uint64_t pauseOrUnpauseProcess(unsigned int processPid);
uint64_t getProcessInfo(processInfo * info);
uint64_t killProcess(unsigned int processPid);
uint64_t niceProcess(uint8_t pid, int delta);
uint64_t renounceCpu();

#endif