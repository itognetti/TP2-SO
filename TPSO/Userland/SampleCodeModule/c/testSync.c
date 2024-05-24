#include <stdint.h>
#include <stdio.h>
#include "./include/syscalls.h"
#include "./include/testUtils.h"
#include "./include/shell.h"

#define SEMAPHORE_NAME "semaphore"
#define PAIR_PROCESS_COUNT 2

uint64_t  sharedVariable; // shared memory

void incrementSlowly(int64_t *ptr, int64_t increment) {
    uint64_t temp = *ptr;
    my_yield(); // This makes the race condition highly probable
    temp += increment;
    *ptr = temp;
}

void performIncrement(int64_t loopCount, int8_t incrementValue, int8_t useSemaphore) {
    for (uint64_t i = 0; i < loopCount; i++) {
        if (useSemaphore)
            my_sem_wait(SEMAPHORE_NAME);
        incrementSlowly(&sharedVariable, incrementValue);
        if (useSemaphore)
            my_sem_post(SEMAPHORE_NAME);
    }
}

uint64_t incrementProcess(uint64_t argCount, char *args[]) {
    if (argCount != 3)
        return -1;

    uint64_t loopCount = satoi(args[0]);
    int8_t incrementValue = satoi(args[1]);
    int8_t useSemaphore = satoi(args[2]);

    if (loopCount <= 0 || incrementValue == 0 || useSemaphore < 0)
        return -1;

    if (useSemaphore && !my_sem_open(SEMAPHORE_NAME, 1)) {
        printf("test_sync: ERROR opening semaphore\n");
        return -1;
    }

    performIncrement(loopCount, incrementValue, useSemaphore);

    if (useSemaphore)
        my_sem_close(SEMAPHORE_NAME);

    return 0;
}

uint64_t createProcesses(char *args[], uint64_t *processIds) {
    char *argsDecrement[] = {args[0], "-1", args[1], NULL};
    char *argsIncrement[] = {args[0], "1", args[1], NULL};

    for (uint64_t i = 0; i < PAIR_PROCESS_COUNT; i++) {
        processIds[i] = my_create_process("incrementProcess", 3, argsDecrement);
        processIds[i + PAIR_PROCESS_COUNT] = my_create_process("incrementProcess", 3, argsIncrement);
    }

    return 0;
}

void waitForProcesses(uint64_t *processIds) {
    for (uint64_t i = 0; i < PAIR_PROCESS_COUNT; i++) {
        my_wait(processIds[i]);
        my_wait(processIds[i + PAIR_PROCESS_COUNT]);
    }
}

uint64_t testSynchronization(uint64_t argCount, char *args[]) {
    if (argCount != 2)
        return -1;
    uint64_t processIds[2 * PAIR_PROCESS_COUNT];
    
    sharedVariable = 0;

    createProcesses(args, processIds);
    waitForProcesses(processIds);

    char *ptrChar = (char *)&sharedVariable;

    //printf("Final value: %d\n", sharedVariable);//el otro printf
    printf("Final value: ");
    printf(ptrChar);
    return 0;
}