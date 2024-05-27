#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <tasks.h>
#include <interrupts.h>
#include <types.h>
#include <queue.h>
#include <locks.h>

#define MAX_SEM 40
#define MAX_PROCESS 20

#define INITIAL_ID  5

#define DONE 0
#define SEM_ID_EXISTS_ERROR -1
#define INVALID_SEM_ID_ERROR -2
#define NO_SPACE_ERROR -3

#define SEM_UNBLOCK 0
#define SEM_BLOCK 1

typedef struct semaphoreData{
	unsigned int id;
	unsigned int value;
	unsigned int lock;
	queueData queue;
}semaphoreData;

int createSemaphore(unsigned int id, unsigned int value);
void destroySemaphore(unsigned int id);
int findSemaphore(unsigned int id);
unsigned int waitSemaphore(unsigned int id);
int getAvailableSemaphore();
int makeSemaphoreAvailable(unsigned int value);
uint64_t getSemaphoreInfo(semaphoreInfo * info);
unsigned int getSemaphoreBlockedProcess(unsigned int pos, unsigned int * blockedPIDS);
unsigned int getSemaphoreBlockedProcessByID(unsigned int semaphoreID, unsigned int * blockedPIDS);
unsigned int signalSemaphore(unsigned int ID);

#endif