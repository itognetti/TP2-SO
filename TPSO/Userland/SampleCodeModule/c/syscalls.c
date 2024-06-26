// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/syscalls.h"

void write(char *string, int row, int col, int color){
    _syscall(SYS_WRITE, string, row, col, color);
}

void read(int fd, char *buffer, int length){
    _syscall(SYS_READ, fd, buffer, length);
}

void clearScreen(){
    _syscall(SYS_CLEAR);
}

void getTime(date currentDate){
    _syscall(SYS_TIME, currentDate);
}

void hold(int time){
    _syscall(SYS_HOLDER, time);
}

void beep(uint32_t beepTime){
    _syscall(SYS_BEEP, beepTime);
}

void getRegisters(){
    _syscall(SYS_INFO_REG);
}

void putPixel(int row, int col, int color){
    _syscall(SYS_DRAW, row, col, color);
}

uint64_t memoryManStatus(uint64_t * buffer){
    return _syscall(SYS_MEMORY_MAN_STATUS , buffer);
}

uint64_t alloc(int len){
    return _syscall(SYS_ALLOC, len);
}

uint64_t freeMem(void * ptr){
    return _syscall(SYS_FREE, ptr);
}

uint64_t readPipe(int pipeId, char * dest, int count){
    return _syscall(SYS_READ_PIPE, pipeId, dest, count);
}

uint64_t writePipe(int pipeId, const char * src, int count){
    return _syscall(SYS_WRITE_PIPE, pipeId, src, count);
}

uint64_t pipeInfo(pipesInfo* info){
    return _syscall(SYS_INFO_PIPE, info);
}

void destroyPipe( int pipeId){
    _syscall(SYS_DESTROY_PIPE, pipeId);
}

uint64_t registerPipeAvailable(){
    return _syscall(SYS_REGISTER_PIPE_AVAILABLE);
}

uint64_t signalSemaphore(uint64_t semId){
    return _syscall(SYS_SEM_SIGNAL, semId);
}

void destroySemaphore(uint64_t semId){
    _syscall(SYS_SEM_DESTROY, semId);
}

uint64_t waitSemaphore(uint64_t semId){
    return _syscall(SYS_SEM_WAIT, semId);
}

uint64_t registerSemaphore(uint64_t semId, unsigned int val){
    return _syscall(SYS_SEM_REG, semId, val);
}

uint64_t registerSemaphoreAvailable(unsigned int val){
    return _syscall(SYS_SEM_REG_AVAILABLE, val);
}

uint64_t getPid(){
    return _syscall(SYS_GET_PID);
}

void waitChildren(){
    _syscall(SYS_CHILD_WAIT);
}

uint64_t registerChildProcess(uint64_t entryP, uint8_t input, uint8_t output, char ** arg0){
    return _syscall(SYS_CHILD_PROC_REGISTER, entryP, input, output, arg0);
}

uint64_t killProcess(unsigned int processPid){
    return _syscall(SYS_KILL_PROC, processPid);
}

uint64_t pauseOrUnpauseProcess(unsigned int processPid){
    return _syscall(SYS_PAUSE_OR_UNPAUSE_PROC, processPid);
}

uint64_t registerProcess(uint64_t entrypoint, uint8_t input, uint8_t output, char ** arg0){
    return _syscall(SYS_REGISTER_PROC, entrypoint, input, output, arg0);
}

uint64_t niceProcess(uint8_t pid, int delta){
    return _syscall(SYS_NICE, pid, delta);
}

uint64_t getProcessInfo(processInfo * info){
    return _syscall(SYS_INFO_PROC, info);
}

uint64_t printMem(uint64_t pos, char * buffer){
    return _syscall(SYS_PRINT_MEMORY, pos, buffer);
}

uint64_t registerPipe(unsigned int pipeId){
    return _syscall(SYS_REGISTER_PIPE, pipeId);
}

uint64_t renounceCpu(){
    return _syscall(SYS_RENOUNCE_CPU);
}

uint64_t aliveProcess(unsigned int processPid){
    return _syscall(SYS_ALIVE_PROC, processPid);
}