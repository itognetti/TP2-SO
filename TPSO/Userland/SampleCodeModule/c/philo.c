#include "../include/philo.h"

void philo() {
    end = FALSE;
    currentCount = 0;

    destroySemaphore(MUTEX);
    destroySemaphore(PRINT_MUTEX);
    registerSemaphore(MUTEX, 1);
    registerSemaphore(PRINT_MUTEX, 1);

    for(int i = 0 ; i < MIN_PHILOSOPHERS; i++){
        addPhilo();
    }

    char buffer = 0;
    while(!end){
        read(STDIN, &buffer, 1);
        switch(buffer){
            case 'a':
                addPhilo(); break;
            case 'r':
                removePhilo(); break;
            case 'q':
                end = TRUE; break;
        }
    }
    cleanUp();

}

void addPhilo(){
    waitSemaphore(MUTEX);
    if(currentCount == MAX_PHILOSOPHERS){
        printf("MAX PHILOSOPHERS REACHED\n");
    }else{
        state[currentCount] = THINKING;
        semaphore[currentCount] = registerSemaphoreAvailable(0);
        safe[currentCount] = registerSemaphoreAvailable(1);

        char string[12] = { "philosopher" };
        char ** philos = { 0 };

        char ** args = (char **) alloc(3 * sizeof(char *));
        char * buf = int64ToStringConverter(currentCount);

        args[0] = (char *) (intptr_t) strncpy(args[0], string, 12);
        args[1] = buf;
        philos = args;
        PIDS[currentCount] = registerChildProcess((uint64_t) &philosopher, STDIN, FOREGROUND, philos);
        if(PIDS[currentCount] <= 0) {
            println("Error creating philosopher");
            return;
        }
        currentCount++;
    }
    signalSemaphore(MUTEX);
}

void removePhilo(){
    if(currentCount == MIN_PHILOSOPHERS){
        printf("MIN PHYLOS REACHED\n");
        return;
    }

    waitSemaphore(safe[currentCount-1]);
    waitSemaphore(MUTEX);

    currentCount--;
    destroySemaphore(safe[currentCount]);
    killProcess(PIDS[currentCount]);
    destroySemaphore(semaphore[currentCount]);
    
    signalSemaphore(MUTEX);
}

void philosopher(char ** num){
    int i = (int) _atoi(num[1]);
    while(!end) {
        waitSemaphore(safe[i]);
        think();
        takeForks(i);
        eat();
        putForks(i);
        signalSemaphore(safe[i]);
    }
}

void takeForks(int i){
    waitSemaphore(MUTEX);
    state[i] = HUNGRY;
    test(i);
    signalSemaphore(MUTEX);
    waitSemaphore(semaphore[i]);
}

void putForks(int i) {
    waitSemaphore(MUTEX);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    signalSemaphore(MUTEX);
}


void test(int i){
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        signalSemaphore(semaphore[i]);
    }
}

void eat(){
    for(int i = 0; i < 5000000; i++);

    waitSemaphore(PRINT_MUTEX);
    for(int i = 0; i < currentCount; i++) {
        if(state[i] == EATING){
            printf("E");
        }else{
            printf(".");
        }
    }
    println("");
    signalSemaphore(PRINT_MUTEX);
}

void think(){
    for(int i = 0; i < 5000000; i++);
}

void cleanUp(){
    for(int i = 0 ; i < currentCount; i++){
        killProcess(PIDS[i]);
        destroySemaphore(semaphore[i]);
        destroySemaphore(safe[i]);
    }
    destroySemaphore(MUTEX);
    destroySemaphore(PRINT_MUTEX);
}