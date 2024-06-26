#ifndef LIBIO_H
#define LIBIO_H

#include <stdint.h>
#include "../include/syscalls.h"
#include <date.h>
#include <colors.h>
#include <inttypes.h>
#include <string.h>

#define DEFAULT_COLOR 0xFFFFFF //WHITE

void printIn(char *string, int row, int col, int color);
void printf(char *string);
void println(char *string);
void printColored(char *string, int color);
void printlnColored(char * string, int color);
char getChar();
void putChar(char c);
void deleteChar();
void scanf(char *buffer, int bufferSize);
void printTime(date currentDate);
int getRandomSign();  
char * int64ToStringConverter(int64_t number);
void invertedString(char * string, int len);
uint64_t _atoi(char * string);
int _strncpy(char * dest, const char * src, unsigned int n);
int isNum(char * string);

#endif
