#ifndef _PIPES_H_
#define _PIPES_H_

#include <semaphore.h>
#include <memoryManager.h>
#include <stdint.h>
#include <stdbool.h>
#include <types.h>
#include <lib.h>

#define MAX_PIPES 20
#define PIPE_SIZE 1024
#define NULL 0
#define INITIAL_PIPE 10 
#define INVALID_PIPE_ID -1
#define EOF -1

typedef struct pipeRecord{
	unsigned int pipeID;

	int readSemID;
	int writeSemID;

	unsigned int writePos;
	unsigned int readPos;
	uint8_t * pipe;
	unsigned int qty;

	uint8_t eof;
}pipeRecord;

static pipeRecord pipeList[MAX_PIPES];
static unsigned int pipesQty = 0;

int createPipe(unsigned int ID);
int findPipe(unsigned int ID);
int findAvailablePipe();
int createAvailablePipe();
void destroyPipe(unsigned int ID);
void signalEOF(unsigned int ID);
int readFromPipe(unsigned int ID, char * dest, unsigned int count);
int writeToPipe(unsigned int ID, const char * src, unsigned int count);
uint64_t getPipeInfo(pipesInfo * info);

#endif