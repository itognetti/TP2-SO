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
