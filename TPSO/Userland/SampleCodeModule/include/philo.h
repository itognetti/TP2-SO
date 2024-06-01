#ifndef PHILO_H
#define PHILO_H

#include <syscalls.h>
#include <libIO.h>

#define MAX_PHILOSOPHERS 15
#define MIN_PHILOSOPHERS 5
#define LEFT    (i + currentCount - 1) % currentCount
#define RIGHT   (i + 1) % currentCount
#define THINKING    0
#define HUNGRY      1
#define EATING      2

#define TRUE 1
#define FALSE 0

#define FOREGROUND 1

#define MUTEX 777
#define PRINT_MUTEX 888

typedef int sem_t;
int end = FALSE;

int state[MAX_PHILOSOPHERS] = { 0 };
sem_t semaphore[MAX_PHILOSOPHERS] = { 0 };
sem_t safe[MAX_PHILOSOPHERS] = { 0 };
int PIDS[MAX_PHILOSOPHERS] = { 0 };
int currentCount = 0;

void philo();
void addPhilo();
void removePhilo();
void philosopher(char ** num);
void takeForks(int i);
void putForks(int i);
void test(int i);
void eat();
void think();
void cleanUp();

#endif