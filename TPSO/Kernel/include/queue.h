#ifndef	QUEUE_H
#define QUEUE_H

#include <memoryManager.h>
#include <stdint.h>
#include <stdbool.h>

#define NULL 0

typedef struct queueData{
    uint64_t * array;
    unsigned int readIndex;
    unsigned int writeIndex;
    unsigned int qty;
    unsigned int size;
}queueData;

void createQueue(queueData * queue, unsigned int size);
void enqueue(queueData * queue, uint64_t elem);
uint64_t dequeue(queueData * queue);
unsigned int getQueueSize(queueData * queue);
uint8_t containsQueue(queueData * queue, uint64_t elem);
void destroyQueue(queueData * queue);
void queueIterator(queueData * queue,unsigned int * ptr);
uint8_t queueHasNext(queueData * queue, unsigned int * ptr);
uint64_t queueNext(queueData * queue, unsigned int * ptr);

#endif