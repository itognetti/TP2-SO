#ifndef CHILDPROCESS_H
#define CHILDPROCESS_H

#include <syscalls.h>
#include <tasks.h>
#include <interrupts.h>

#define MAX_WAIT_TASKS 30
#define NOT_TRACKING 0
#define RUNNING 1
#define FINISHED 2

typedef struct waitData {
    unsigned int fatherPid;
    unsigned int childPid;
    uint8_t state;
} waitData;

uint8_t hasChildProcesses(unsigned int pid);
void assignChildProcess(unsigned int fatherPid, unsigned int childPid);
void removeChildProcess(unsigned int pid);
void waitForChildProcess();
uint8_t childProcessesCompleted(unsigned int pid);
unsigned int createChildTask(uint64_t entryP, uint8_t input, uint8_t output, char ** arg0);
void reportChildProcessFinished(unsigned int pid);

#endif