#include <testSync.h>

void incrementSlowly(int64_t *ptr, int64_t increment) {
    int aux;
    aux = *ptr;
    aux += increment;
    for(int i = 0; i < 40000; i++);
    *ptr = aux;
}

void incrementProcess(uint64_t argCount, char *args[]) {
    int increment = 1;
    for(int i = 0; i < MAX; i++){
        if(args[1][0] == 's'){
            waitSemaphore(SEM_ID);
        }
        incrementSlowly(&global, increment);
        if(args[1][0] == 's'){
            signalSemaphore(SEM_ID);
        }
    } 
}

void testSynchronization(uint64_t argCount, char *args[]) {
    int semInUse = satoi(args[1]);
    if(semInUse){
        destroySemaphore(SEM_ID);
        if(registerSemaphore(SEM_ID, 1) != 0){
            printf("testSync: Error while opening the semaphore");
            return ;
        }
    }
    char *argv[] = {"sem", NULL, NULL};

    global = 0;

    if(semInUse){
        argv[1] = "s";
    }

    for(int i = 0; i < TOTAL_PAIR_PROC; i++){
        int retValue = registerChildProcess((uint64_t) &incrementProcess, STDIN, BACKGROUND, (uint64_t) argv);
        if (retValue <= 0){
            printf("testSync: Error creating children");
        }
    }

    waitChildren();

    if(semInUse){
        destroySemaphore(SEM_ID);
    }

    char * expectedValue = int64ToStringConverter(MAX * TOTAL_PAIR_PROC);
    char * realValue = int64ToStringConverter(global);

    printf("testSync: Global expected value");
    println(expectedValue);
    printf("testSync: Global real value");
    println(realValue);

    return 0;
}