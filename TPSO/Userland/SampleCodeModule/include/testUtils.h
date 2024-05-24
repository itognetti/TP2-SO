#ifndef TESTUTILS_H
#define TESTUTILS_H
#include <stdint.h>
uint32_t generateUniformRandom(uint32_t max);
int64_t satoi(char *str);
uint32_t generateRandomUint();
uint8_t checkMemory(void *start, uint8_t value, uint32_t size);
#endif