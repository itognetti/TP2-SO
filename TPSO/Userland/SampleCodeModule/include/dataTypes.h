#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H
#include <stdint.h>

typedef struct ProcessInfo {
    char * processName; 
    unsigned int processId;
    uint8_t processState;
    uint8_t processPriority;
    uint64_t processStack;
    uint64_t processRsp;
    uint64_t processPickd;
    uint8_t processScreen;
} ProcessInfo;

typedef struct SemaphoreInfo {
    unsigned int semaphoreId;
    unsigned int semaphoreValue;
    unsigned int numBlockedProcesses;
    unsigned int blockedProcessIds[10];
} SemaphoreInfo;

typedef struct PipeInfo {
    unsigned int pipeId;
    unsigned int pipeUsage;
    unsigned int numBlockedReaders;
    unsigned int blockedReaderIds[10];
    unsigned int numBlockedWriters;
    unsigned int blockedWriterIds[10];
} PipeInfo;

#endif