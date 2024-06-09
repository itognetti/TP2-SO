// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../include/semaphore.h"

static semaphoreData semaphoreDataArray[MAX_SEM] = {{0}};
static unsigned int semCount = 0;

unsigned int getSemaphoreBlockedProcessByID(unsigned int semaphoreID, unsigned int * blockedPIDS){
	int pos = findSemaphore(semaphoreID);
	if(pos == INVALID_SEM_ID_ERROR)
		return INVALID_SEM_ID_ERROR;

	return getSemaphoreBlockedProcess((unsigned int)pos, blockedPIDS);
}

unsigned int waitForSemaphore(unsigned int semID) {
    int semIndex = findSemaphore(semID);
    if (semIndex == INVALID_SEM_ID_ERROR) {
        return INVALID_SEM_ID_ERROR;
    }

    semaphoreData *sem = &semaphoreDataArray[semIndex];
    lock(&(sem->lock));

    if (sem->value > 0) {
        sem->value--;
    } else {
        int currentPID = getCurrentPID();
        changeState(currentPID, WAITING_FOR_SEM);
        enqueue(&(sem->queue), currentPID);
        unlock(&(sem->lock));
        forceChangeTask();
        return true;
    }

    unlock(&(sem->lock));
    return true;
}
int initializeSemaphore(unsigned int semID, unsigned int initialValue) {
    if (semID == 0 || semCount == MAX_SEM) {
        return (semID == 0) ? INVALID_SEM_ID_ERROR : NO_SPACE_ERROR;
    }

    int freeIndex = -1;

    for (int i = 0; i < MAX_SEM; i++) {
        if (semaphoreDataArray[i].id == semID) {
            return INVALID_SEM_ID_ERROR;
        }
        if (freeIndex == -1 && semaphoreDataArray[i].id == 0) {
            freeIndex = i;
        }
    }

    if (freeIndex == -1) {
        return NO_SPACE_ERROR;
    }

    createQueue(&(semaphoreDataArray[freeIndex].queue), MAX_PROCESS);
    semaphoreDataArray[freeIndex].id = semID;
    semaphoreDataArray[freeIndex].value = initialValue;
    semCount++;

    return DONE;
}
void removeSemaphore(unsigned int semID) {
    int semIndex = findSemaphore(semID);
    if (semIndex == INVALID_SEM_ID_ERROR) {
        return;
    }
    lock(&(semaphoreDataArray[semIndex].lock));
    semaphoreDataArray[semIndex].id = 0;
    semaphoreDataArray[semIndex].value = 0;
    unlock(&(semaphoreDataArray[semIndex].lock));
    destroyQueue(&(semaphoreDataArray[semIndex].queue));
    return;
}

int findSemaphore(unsigned int semID) {
    for (semaphoreData *sem = semaphoreDataArray; sem < semaphoreDataArray + MAX_SEM; sem++) {
        if (sem->id == semID) {
            return sem - semaphoreDataArray;
        }
    }
    return INVALID_SEM_ID_ERROR;
}



int setSemaphoreWithValue(unsigned int value) {
    int id = getAvailableSemaphore();
    
    if (id < 0) {
        return id; 
    }

    int result = initializeSemaphore(id, value);
    return (result == DONE) ? id : result; 
}

uint64_t getSemaphoreInfo(semaphoreInfo * info){
	int j = 0;
	for(int i = 0; i < MAX_SEM; i++){
		if(semaphoreDataArray[i].id != 0){
			info[j].id = semaphoreDataArray[i].id;
			info[j].value = semaphoreDataArray[i].value;
			info[j].numBlocked = getSemaphoreBlockedProcess(i, info[j].blockedPIDS);
			j++;
		}
	}
	return j;
}

unsigned int getSemaphoreBlockedProcess(unsigned int pos, unsigned int *blockedPIDS) {
    if (pos >= MAX_SEM) {
        return 0;
    }
    int queueSize = getQueueSize(&(semaphoreDataArray[pos].queue));
    if (queueSize > 0) {
        unsigned int blockedProcesses[queueSize];
        queuePeekAll(&(semaphoreDataArray[pos].queue), blockedProcesses);
        for (int i = 0; i < queueSize; i++) {
            blockedPIDS[i] = blockedProcesses[i];
        }
        return queueSize;
    }
    return 0;
}



unsigned int signalSemaphore(unsigned int ID){
	int pos = findSemaphore(ID);
	if(pos == INVALID_SEM_ID_ERROR){
		return INVALID_SEM_ID_ERROR;
	}

	lock(&(semaphoreDataArray[pos].lock));
	if(getQueueSize(&(semaphoreDataArray[pos].queue)) > 0){
		unsigned int blockedPID = dequeue(&(semaphoreDataArray[pos].queue));
		changeState(blockedPID, ACTIVE_PROCESS);
	}else{
		semaphoreDataArray[pos].value++;
	}
	unlock(&(semaphoreDataArray[pos].lock));
	return true;
}

int getAvailableSemaphore(){
	if(semCount == MAX_SEM)
		return NO_SPACE_ERROR;

	uint8_t found = 0;
	int id = 10;  

	while(!found){
		found = 1;
		for(int i = 0 ; i < MAX_SEM; i++){
			if(semaphoreDataArray[i].id == id){
				found = 0;
				id++;
				break;
			}
		}
	}
	return id;
}
