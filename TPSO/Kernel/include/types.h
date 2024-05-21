#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>

typedef struct processInfo{
    char * name;
    unsigned int id;
    uint8_t state;
    uint8_t priority;
    uint64_t stack;
    uint64_t rsp;
    uint64_t pickd;
    uint8_t screen;
} processInfo;

typedef struct pipesInfo{
	unsigned int id;
	unsigned int usage;
	unsigned int readNumBlocked;
	unsigned int readBlockedPIDS[10];
	unsigned int writeNumBlocked;
	unsigned int writeBlockedPIDS[10];
}pipesInfo;

typedef struct semaphoreInfo{
	unsigned int id;
	unsigned int value;
	unsigned int numBlocked;
	unsigned int blockedPIDS[10];
}semaphoreInfo;

#endif