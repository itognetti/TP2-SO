#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

#define MAX_DIGITS 20

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
void printCharColor(uint8_t character, uint8_t attribute);
void printStringColor(const char * string, uint16_t attribute);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
void uintToBaseN(uint64_t number, char* result, int base);
uint64_t turnToBaseN(uint64_t value, int base, char *buffer, int bufferLength);

#endif