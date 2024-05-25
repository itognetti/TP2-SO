#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

typedef struct processInfo {
    char * processName; 
    unsigned int processId;
    uint8_t processState;
    uint8_t processPriority;
    uint64_t processStack;
    uint64_t processRsp;
    uint64_t processPickd;
    uint8_t processScreen;
} processInfo;

typedef struct semaphoreInfo {
    unsigned int semaphoreId;
    unsigned int semaphoreValue;
    unsigned int numBlockedProcesses;
    unsigned int blockedProcessIds[10];
} semaphoreInfo;

typedef struct pipesInfo{
    unsigned int pipeId;
    unsigned int pipeUsage;
    unsigned int numBlockedReaders;
    unsigned int blockedReaderPids[10];
    unsigned int numBlockedWriters;
    unsigned int blockedWriterPids[10];
} pipesInfo;

#endif