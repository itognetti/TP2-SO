#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <date.h>

#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_CLEAR 2
#define SYS_TIME 3
#define SYS_HOLDER 4
#define SYS_BEEP 5
#define SYS_INFO_REG 6
#define SYS_DRAW 7

extern void _syscall(uint64_t syscall, ...);

void write(char *string, int row, int col, int color);
void read(char *buffer, int length);
void clearScreen();
void getTime(date currentDate);
void hold(int time);
void beep(uint32_t beepTime);
void getRegisters();
void putPixel(int row, int col, int color);

#endif