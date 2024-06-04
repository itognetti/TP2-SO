#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <stdint.h>
#include "../include/syscalls.h"
#include <libIO.h>

uint32_t getUint();
uint32_t getUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
void bussyWait(uint64_t n);
void endlessLoop();
void endlessLoopPrint(uint64_t wait);

#endif