#include "../include/tasks.h"

static PCB tasks[TOTAL_TASKS];

static unsigned int newPID = 1;					// process identifier
	
static unsigned int currentTask = 0;
static unsigned int currentRemainingTicks = 0;			// amount of timer ticks remaining for the current process
static unsigned int currentDimTasks = 0;

static unsigned int idleTaskPID = 1;

static char * idleArg[] = {"idle", NULL};

void idleTask(){
	while(true)
		_hlt();
}

void enableMultiTasking(){
	idleTaskPID = addTask((uint64_t)&idleTask, STDIN, BACKGROUND, DEFAULT_PRIORITY, IMMORTAL,idleArg);
	changeState(idleTaskPID, PAUSED_PROCESS);		// pause until it's required
	executeCurrentTask();
}

unsigned int getCurrentPID(){
	return tasks[currentTask].PID;
}
uint64_t getRSP(){
	return tasks[currentTask].stackPointer;
}
uint64_t getSS(){
	return tasks[currentTask].stackSegment;
}
uint8_t getCurrentOutput(){
	return tasks[currentTask].output;
}
uint8_t getCurrentInput(){
	return tasks[currentTask].input;
}

uint8_t getState(unsigned int PID){
	int pos = findTask(PID);
	if(pos == NO_TASK_FOUND)
		return NO_TASK_FOUND;
	
	return tasks[pos].state;
}

int findTask(unsigned int PID){
	for(int i = 0; i < TOTAL_TASKS; i++){
		if(tasks[i].PID == PID && tasks[i].state != DEAD_PROCESS)
			return i;
	}	
	return NO_TASK_FOUND;
}

uint64_t buildStack(uint64_t entrypoint, char ** arg0, uint64_t stackEnd){
	uint64_t stackStart = stackEnd + STACK_SIZE;
	stackStart -= stackStart % ALINGMENT;

	*(STACK_POS(RDI_POS)) = (uint64_t) arg0;

	for(int i = 7 ; i < 21 ; i++){
		if(i != 12)
			*(STACK_POS(i * 8)) = 0;
	}

	*(STACK_POS(IP_POS)) = entrypoint;	
	*(STACK_POS(CS_POS)) = CS_VALUE;				
	
	*(STACK_POS(FLAGS_POS)) = FLAG_VALUES;
	
	*(STACK_POS(SP_POS)) = stackStart - RET_POS;
	*(STACK_POS(SS_POS)) = SS_VALUE;
	
	*(STACK_POS(RET_POS)) = (uint64_t) &removeCurrentTask;	

	return stackStart;
}

int addTask(uint64_t entrypoint, uint8_t input, uint8_t output, uint8_t priority, uint8_t immortal, char ** arg0){
	if(currentDimTasks >= TOTAL_TASKS){
		return ERROR_NO_SPACE_FOR_TASK;
	}
	currentDimTasks++;

	int pos;
	for(pos = 0; tasks[pos].state != DEAD_PROCESS; pos++);

	uint8_t * stackEnd = m_malloc(STACK_SIZE);

	if(stackEnd == NULL)
		return ERROR_NO_SPACE_FOR_TASK;

	uint8_t * stackStart = (uint8_t *) buildStack(entrypoint, arg0, (uint64_t) stackEnd);

	tasks[pos].stackPointer = (uint64_t) stackStart - STACK_POINT_OF_ENTRY; 
	tasks[pos].stackSegment = SS_VALUE;							

	tasks[pos].PID = newPID++;
	tasks[pos].state = ACTIVE_PROCESS;
	tasks[pos].priority = priority;
	tasks[pos].immortal = immortal;

	tasks[pos].stackStart = stackStart;
	tasks[pos].stackEnd = stackEnd;

	tasks[pos].params = arg0;
	tasks[pos].ticks = 1;

	tasks[pos].input = input;
	tasks[pos].output = output;

	return tasks[pos].PID;
}

void freeParams(char ** params){
	if(params == NULL)
		return;

	for(int i = 0; params[i] != NULL; i++){
		m_free(params[i]);
	}
	m_free(params);
}

void destroyProcess(unsigned int pos){
	signalFinished(tasks[pos].PID);
	freeParams(tasks[pos].params);
	tasks[pos].state = DEAD_PROCESS;
	currentDimTasks--;
	m_free(tasks[pos].stackEnd);
}

void removeCurrentTask(){
	_cli();

	destroyProcess(currentTask);
	
	uint8_t out = tasks[currentTask].output;
	if(out != STDOUT){
		signalEOF(out);
	}
	forceChangeTask();
}

void forceChangeTask(){
	currentRemainingTicks = tasks[currentTask].priority + 1;
	triggerTimerTick();
}

void changeStateIf(uint8_t oldState, uint8_t newState){
	for(int i = 0; i < TOTAL_TASKS; i++){
		if(tasks[i].state != DEAD_PROCESS && tasks[i].state == oldState){
			tasks[i].state = newState;
		}
	}
}

void changeState(unsigned int PID, uint8_t newState){
	int pos = findTask(PID);
	if(pos == NO_TASK_FOUND)
		return;

	tasks[pos].state = newState;
}

void pauseScreenProcess(unsigned int screen){
	for(int i = 0; i < TOTAL_TASKS; i++){
		if(tasks[i].state != WAITING_FOR_CHILD && tasks[i].state != DEAD_PROCESS && tasks[i].output == screen){
			tasks[i].state = tasks[i].state == PAUSED_PROCESS ? ACTIVE_PROCESS : PAUSED_PROCESS;
		}
	}
}

void killScreenProcesses(){
	uint8_t currentTaskKilled = false;
	for(int i = 0; i < TOTAL_TASKS; i++){
		if(tasks[i].state != DEAD_PROCESS &&  tasks[i].immortal != IMMORTAL ){
			destroyProcess(i);

			if(i == currentTask){
				currentTaskKilled = true;
			}
		}
	}
	if(currentTaskKilled){
		forceChangeTask();
	}
}

int pauseOrUnpauseProcess(unsigned int pid){
	int pos = findTask(pid);
	if(pos < 0)
		return NO_TASK_FOUND;

	if(tasks[pos].immortal)
		return TASK_NOT_ALTERED;

	tasks[pos].state = tasks[pos].state==PAUSED_PROCESS ? ACTIVE_PROCESS : PAUSED_PROCESS; 

	if(pos == currentTask){
		forceChangeTask();
	}

	return TASK_ALTERED;
}

int removeTask(unsigned int PID){
	int pos = findTask(PID);
	if(pos < 0)
		return NO_TASK_FOUND;

	if(tasks[pos].immortal)
		return TASK_NOT_ALTERED;

	destroyProcess(pos);

	if(pos == currentTask){
		forceChangeTask();
	}

	return TASK_ALTERED;
}

unsigned int changePriority(unsigned int PID, int delta){
	int pos = findTask(PID);
	if(pos < 0)
		return NO_TASK_FOUND;

	if(delta > MAX_PRIORITY){
		tasks[pos].priority = MAX_PRIORITY;
    }else if(delta < 1){
		tasks[pos].priority = DEFAULT_PRIORITY;
    }else{	
		tasks[pos].priority = delta;
    }
	return true;
}

uint8_t enoughTimeLeft(){
	if(currentRemainingTicks < tasks[currentTask].priority - 1){
		tasks[currentTask].ticks++;
		currentRemainingTicks++;
		return true;
	}
	return false;
}

uint64_t nextTask(uint64_t stackPointer, uint64_t stackSegment){
	tasks[currentTask].stackPointer = stackPointer;			
	tasks[currentTask].stackSegment = stackSegment;
	
	char found = 0;
	unsigned int counter = 0;
	while( !found && counter < currentDimTasks ){			
		
		currentTask = (currentTask +  1) % TOTAL_TASKS;

		if(tasks[currentTask].state != DEAD_PROCESS)		 
			counter++;

		if(tasks[currentTask].state == ACTIVE_PROCESS){
			found = 1;
			tasks[currentTask].ticks++;
		}
	}

	if(counter == currentDimTasks){				// if all tasks are paused -> unpause idle task
		currentTask = findTask(idleTaskPID);
		changeState(idleTaskPID, ACTIVE_PROCESS);
	}
	else if(tasks[currentTask].PID != idleTaskPID){			// if idle task is not paused -> pause it
		changeState(idleTaskPID, PAUSED_PROCESS);
	}

	currentRemainingTicks = 0;

	return tasks[currentTask].stackPointer;
}

int getProcessInfo(processInfo * info){
	int j=0;
	for(int i=0; i<TOTAL_TASKS; i++){
		if(tasks[i].state != DEAD_PROCESS){
			if(tasks[i].params !=NULL){
				info[j].name = tasks[i].params[0];		
			}
			info[j].id = tasks[i].PID;
			info[j].state = tasks[i].state;
			info[j].priority = tasks[i].priority;
			info[j].stack = (uint64_t) tasks[i].stackStart;
			info[j].rsp = tasks[i].stackPointer;
			info[j].pickd = tasks[i].ticks;
			info[j].screen = tasks[i].output;

			j++;
		}
	}
	return j;
}