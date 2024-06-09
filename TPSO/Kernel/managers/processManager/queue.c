// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/queue.h"

void createQueue(queueData * queue, unsigned int size){
    if(size == 0){
        return;
    }
    queue->array = m_malloc(size * sizeof(uint64_t));
    queue->readIndex = 0;
    queue->writeIndex = 0;
    queue->qty = 0;
    queue->size = size;
}

void enqueue(queueData * queue, uint64_t elem){
    if(queue->qty == queue->size){
        return;
    }
    queue->array[queue->writeIndex] = elem;
    queue->writeIndex = (queue->writeIndex + 1) % (queue->size);
    queue->qty++;
}

uint64_t dequeue(queueData * queue){
    if(queue->qty == 0){
        return (uint64_t) 0;
    }
    uint64_t resp = queue->array[queue->readIndex];
    queue->readIndex = (queue->readIndex + 1) % (queue->size);
    queue->qty--;

    return resp;
}


unsigned int getQueueSize(queueData * queue){
    return queue->qty;
}

uint8_t containsQueue(queueData * queue, uint64_t elem){
    int i = queue->readIndex;
    for(int j = 0 ;  j < queue->qty; j++){
        if(queue->array[i] == elem){
            return true;
        }
        i = (i + 1) % queue->size;
    }
    return false;
}

void destroyQueue(queueData * queue){
    m_free(queue->array);
}

void queuePeekAll(queueData * queue,unsigned int * ptr){
    *ptr = queue->readIndex;
}

uint8_t queueHasNext(queueData * queue, unsigned int * ptr){
    if(queue->writeIndex < queue->readIndex){
        return (*ptr < queue->size && *ptr >= queue->readIndex) || (*ptr < queue->writeIndex);
    }   
    return *ptr < queue->writeIndex && *ptr >= queue->readIndex;
}

uint64_t queueNext(queueData * queue, unsigned int * ptr){ 
    if(*ptr >= queue->size){
        return 0;
    }
    uint64_t toReturn = queue->array[*ptr];
    *ptr = ((*ptr) + 1) % queue->size;
    return toReturn;
}