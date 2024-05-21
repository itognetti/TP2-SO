#include "../include/syscalls.h"

void sys_write(char * string, int row, int col, int color){
    if(row == -1 || col == -1){
        print(string, color);
    }
    else{
        printIn(string, row, col, color);
    }
}

void sys_read(char *buffer, int length){
    readKeyboard(buffer, length);
}

void sys_clear(){
    clearScreen();
}

void sys_time(date currentDate){
    getDate(currentDate);
}

void sys_holder(int time){
    holder(time);
}

void sys_beep(uint32_t beepTime){
    beep(beepTime);
}

void sys_info_reg(){
    getRegistersState();
}

void sys_draw(int row, int col, int color){
    putPixel(row, col, color);
}