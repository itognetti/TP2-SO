#include "../include/syscalls.h"

void write(char *string, int row, int col, int color){
    _syscall(SYS_WRITE, string, row, col, color);
}

void read(char *buffer, int length){
    _syscall(SYS_READ, buffer, length);
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

uint64_t alloc(int len){
    return _syscall(SYS_ALLOC, len);
}

uint64_t readPipe(int pipeId, char * dest, int count){
    return _syscall(SYS_READ_PIPE, pipeId, dest, count);
}

uint64_t writePipe(int pipeId, const char * src, int count){
    return _syscall(SYS_WRITE_PIPE, pipeId, src, count);
}

uint64_t pipeInfo(PipeInfo * info){
    return _syscall(SYS_PIPE_INFO, info);
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

uint64_t destroySemaphore(uint64_t semId){
    return _syscall(SYS_SEM_DESTROY, semId);
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

uint64_t waitChildren(){
    return _syscall(SYS_CHILD_WAIT);
}

uint64_t registerChildProcess(uint64_t entryP, uint8_t input, uint8_t output, uint64_t arg0){
    return _syscall(SYS_CHILD_PROC_REGISTER);
}