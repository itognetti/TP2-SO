#include <pipes.h>

static pipeRecord pipeList[MAX_PIPES];
static unsigned int pipeCount  = 0;

int createPipe(unsigned int id){
	if(id == 0)				
		return INVALID_PIPE_ID;
	if(pipeCount  == MAX_PIPES)
		return NO_SPACE_ERROR;

	int freeIndex  = -1;
	for(int i = 0; i < MAX_PIPES; i++){
		if(freeIndex  == -1 && pipeList[i].pipeID == 0){
			freeIndex  = i;
		}
		if(pipeList[i].pipeID == id){
			return INVALID_PIPE_ID;
		}
	}
	int semID1 = makeSemaphoreAvailable(0);
	int semID2 = makeSemaphoreAvailable(PIPE_SIZE);
	if(semID1 == INVALID_SEM_ID_ERROR || semID2 == INVALID_SEM_ID_ERROR){
		destroySemaphore(semID1);
		destroySemaphore(semID2);
		return NO_SPACE_ERROR;
	}

	pipeList[freeIndex ].pipe = m_malloc(PIPE_SIZE);
	if(pipeList[freeIndex ].pipe == NULL){
		destroySemaphore(semID1);
		destroySemaphore(semID2);
		return NO_SPACE_ERROR;
	}
	pipeList[freeIndex ].pipeID = id;
	pipeList[freeIndex  ].readSemID  = semID1;
	pipeList[freeIndex ].writeSemID  = semID2;
	pipeList[freeIndex ].writePos = 0;
	pipeList[freeIndex ].readPos = 0;
	pipeList[freeIndex ].qty = 0;
	pipeList[freeIndex ].eof = 0;

	pipeCount ++;
	return DONE;
}

int findPipe(unsigned int id){
	for(int i = 0; i < MAX_PIPES; i++){
		if(pipeList[i].pipeID == id){
			return i;
		}
	}
	return INVALID_PIPE_ID;
}

int findAvailablePipe(){
	if(pipeCount  == MAX_PIPES)
		return NO_SPACE_ERROR;

	uint8_t found = false;
	int pipeID = 10;

	while(!found){
		found = true;
		for(int i = 0; i < MAX_PIPES; i++){
			if(pipeList[i].pipeID == pipeID){
				found = false;
				pipeID++;
				break;
			}
		}
	}
	return pipeID;
}

int createAvailablePipe() {
    int id = findAvailablePipe();
    if (id == NO_SPACE_ERROR) {
        return NO_SPACE_ERROR;
    }
    return (createPipe(id) == DONE) ? id : NO_SPACE_ERROR;
}

void destroyPipe(unsigned int id){
	int pos = findPipe(id);
	if(pos == INVALID_PIPE_ID){
		return;
	}
	destroySemaphore(pipeList[pos].writeSemID);	
	destroySemaphore(pipeList[pos].readSemID);
	m_free(pipeList[pos].pipe);

	pipeList[pos].readSemID  = 0;
	pipeList[pos].writeSemID  = 0;
	pipeList[pos].pipeID = 0;
	pipeList[pos].writePos = 0;
	pipeList[pos].readPos = 0;
	pipeList[pos].qty = 0;
	pipeList[pos].pipe = 0;

	pipeCount --;
}

void signalEOF(unsigned int id){
	int pos = findPipe(id);
	if(pos == INVALID_PIPE_ID){
		return;
	}
	pipeList[pos].eof = 1;
}

int readFromPipe(unsigned int id, char * buffer, unsigned int count){
	int pos = findPipe(id);
	if(pos == INVALID_PIPE_ID){
		return INVALID_PIPE_ID;
	}
	if(pipeList[pos].eof && pipeList[pos].qty == 0){
		return EOF;
	}
	int i = 0;	
	for(; i < count && !(pipeList[pos].eof && pipeList[pos].qty == 0); i++){
		waitSemaphore(pipeList[pos].readSemID);

		buffer[i] = pipeList[pos].pipe[pipeList[pos].readPos];
		MOD_INCREMENT(pipeList[pos].readPos, PIPE_SIZE);
		pipeList[pos].qty--;

		signalSemaphore(pipeList[pos].writeSemID);
	}
	return i;
}

int writeToPipe(unsigned int id, const char * buffer, unsigned int count){
	int pos = findPipe(id);
	if(pos == INVALID_PIPE_ID)
		return INVALID_PIPE_ID;

	for(int i=0; i<count; i++){
		waitSemaphore(pipeList[pos].writeSemID);
		pipeList[pos].pipe[pipeList[pos].writePos] = buffer[i];
		MOD_INCREMENT(pipeList[pos].writePos, PIPE_SIZE);
		pipeList[pos].qty++;

		signalSemaphore(pipeList[pos].readSemID);
	}
	return count;
}

uint64_t getPipeInfo(pipesInfo * info){
	int activePipeCount  = 0;
	for(int i = 0; i < MAX_PIPES; i++){
		if(pipeList[i].pipeID != 0){
			info[activePipeCount ].id = pipeList[i].pipeID;
			info[activePipeCount ].usage = pipeList[i].qty;
			info[activePipeCount ].readNumBlocked = getSemaphoreBlockedProcessByID(pipeList[i].readSemID, info[activePipeCount ].readBlockedPIDS);
			info[activePipeCount ].writeNumBlocked = getSemaphoreBlockedProcessByID(pipeList[i].writeSemID, info[activePipeCount ].writeBlockedPIDS);
			activePipeCount ++;
		}
	}
	return activePipeCount ;
}