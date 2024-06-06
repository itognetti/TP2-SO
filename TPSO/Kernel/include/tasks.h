#ifndef _TASKS_H
#define _TASKS_H

#include <interrupts.h>
#include <stdint.h>
#include <types.h>
#include <stdbool.h>
#include <syscalls.h>
#include <memoryManager.h>
#include <pipes.h>

#define NO_TASKS 0
#define NO_TASK_FOUND -1
#define TASK_ALTERED 1
#define TASK_NOT_ALTERED -1

#define ERROR_NO_SPACE_FOR_TASK -1
#define ERROR_SCREEN_NOT_AVAILABLE 2

#define DEFAULT_PRIORITY 1
#define IMMORTAL 1
#define MORTAL 0

#define DEAD_PROCESS 0
#define ACTIVE_PROCESS 1 
#define PAUSED_PROCESS 2
#define WAITING_FOR_CHILD 3
#define WAITING_FOR_SEM 4
#define WAITING_FOR_INPUT 5

#define TOTAL_TASKS 20
#define STACK_SIZE 4096

#define MAX_PRIORITY 5

// default values for stack creation
#define FLAG_VALUES 0x202
#define SS_VALUE 0x0				
#define CS_VALUE 0x8

// Posiciones para el armado de stack para cada proceso
										/*		 -=-=STACK=-=-		*/
#define STACK_POINT_OF_ENTRY (21*8)   	/*  	|	RAX, RBX  |		*/
										/*  	|	RCX, etc  |		*/   
#define RDI_POS   (12*8)				/*		---------------		*/ 
#define IP_POS    (6*8)					/*  	|	 RIP	  |		*/				
#define CS_POS 	  (5*8)					/*  	|	  CS	  |		*/
#define FLAGS_POS (4*8)					/*  	|	 RFLAGS	  |		*/
#define SP_POS 	  (3*8)					/*  	|	 RSP	  |		*/
#define SS_POS	  (2*8)					/*  	|	  SS	  |		*/
#define RET_POS 	8 					/*  	|	 RET	  |		*/
#define ALINGMENT 8		 				/*   	 -------------		*/

#define STACK_POS(POS) (uint64_t *) (stackStart - (POS))

// Task info
typedef struct PCB{ //Process Control Block
		unsigned int PID;				// unique identifier > 0

		uint64_t  stackPointer;			// value of rsp 
		uint64_t  stackSegment;  		// value of ss (constant) = 0
		void * stackStart;
		void * stackEnd;
		char ** params;

		uint8_t state;
		uint8_t priority;				// amount of ticks it has to run
		uint8_t immortal;				// whether it can or can't be killed/blocked/paused
		
		uint8_t output;
		uint8_t input;

		uint64_t ticks;					// amount of times the scheduler picked it to run
}PCB;

void idleTask();
void enableMultiTasking();

unsigned int getCurrentPID();
uint64_t getRSP();
uint64_t getSS();
uint8_t getCurrentOutput();
uint8_t getCurrentInput();

uint8_t getState(unsigned int PID);
int findTask(unsigned int PID);
uint64_t buildStack(uint64_t entrypoint, char ** arg0, uint64_t stackEnd);
int addTask(uint64_t entrypoint, uint8_t input, uint8_t output, uint8_t priority, uint8_t immortal, char ** arg0);
void freeParams(char ** params);
void destroyProcess(unsigned int pos);
void removeCurrentTask();
void forceChangeTask();
void changeStateIf(uint8_t old_state, uint8_t new_state);
void changeState(unsigned int PID, uint8_t new_state);
void pauseScreenProcess(unsigned int screen);
void killScreenProcesses();
void pauseScreenProcess(unsigned int screen);
int pauseOrUnpauseProcess(unsigned int pid);
int removeTask(unsigned int PID);
unsigned int changePriority(unsigned int PID, int delta);
uint8_t enoughTimeLeft();
uint64_t nextTask(uint64_t stackPointer, uint64_t stackSegment);
int getProcessInfo(processInfo * info);

#endif