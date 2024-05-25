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

uint64_t readPipe(int pipeID, char * dest, int count){
    return _syscall(SYS_READ_PIPE, pipeID, dest, count);
}

uint64_t writePipe(int pipeID, const char * src, int count){
    return _syscall(SYS_WRITE_PIPE, pipeID, src, count);
}

uint64_t pipeInfo(PipeInfo * info){
    return _syscall(SYS_PIPE_INFO, info);
}

void destroyPipe( int pipeID){
    _syscall(SYS_DESTROY_PIPE, pipeID);
}

uint64_t registerPipeAvailable(){
    return _syscall(SYS_REGISTER_PIPE_AVAILABLE);
}