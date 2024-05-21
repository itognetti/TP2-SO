#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>
#include <font.h>
#include <lib.h>
#include <colors.h>

#define WIDTH 1024
#define HEIGHT 768

void putPixel(uint32_t x, uint32_t y, uint32_t color);
int putCharIn(int character, int row, int col, int color);
void enter();
void backspace();
void putChar(int character, int color);
void clearScreen();
void updateCoordinates();
void printIn(char *string, int row, int col, int color);
void print(char *string, int color);
void println(char *string, int color);
int strlen(char *string);

#endif