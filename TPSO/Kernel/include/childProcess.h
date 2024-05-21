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

static waitData taskList[MAX_WAIT_TASKS] = {{0}};

uint8_t hasChildren(unsigned int pid);
void addChildren(unsigned int fatherPid, unsigned int childPid);
void removeChildren(unsigned int pid);
void waitForChildren();
uint8_t childrenFinished(unsigned int pid);
unsigned int addChildrenTask(uint64_t entryP, uint8_t input, uint8_t output, char ** arg0);
void signalFinished(unsigned int pid);

#endif