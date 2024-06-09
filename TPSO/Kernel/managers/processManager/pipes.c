// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipes.h>

static pipeRecord pipes[MAX_PIPES];
static unsigned int totalPipes  = 0;


int generateAvailablePipeID(){
	if(totalPipes  == MAX_PIPES)
		return NO_SPACE_ERROR;

	uint8_t found = false;
	int pipeID = 10;

	while(!found){
		found = true;
		for(int i = 0; i < MAX_PIPES; i++){
			if(pipes[i].pipeID == pipeID){
				found = false;
				pipeID++;
				break;
			}
		}
	}
	return pipeID;
}


int initializePipe(unsigned int id) {
    if (id == 0 || totalPipes == MAX_PIPES) {
        return INVALID_PIPE_ID;
    }
    int freeIndex = -1;
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i].pipeID == 0) {
            freeIndex = i;
            break;
        }
    }
    if (freeIndex == -1) {
        return NO_SPACE_ERROR;
    }
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i].pipeID == id) {
            return INVALID_PIPE_ID;
        }
    }
    int readSemaphore  = setSemaphoreWithValue(0);
    int writeSemaphore  = setSemaphoreWithValue(PIPE_SIZE);
    if (readSemaphore  == INVALID_SEM_ID_ERROR || writeSemaphore  == INVALID_SEM_ID_ERROR) {
        removeSemaphore(readSemaphore );
        removeSemaphore(writeSemaphore );
        return NO_SPACE_ERROR;
    }
    pipes[freeIndex].pipe = m_malloc(PIPE_SIZE);
    if (pipes[freeIndex].pipe == NULL) {
        removeSemaphore(readSemaphore );
        removeSemaphore(writeSemaphore );
        return NO_SPACE_ERROR;
    }
    pipes[freeIndex].pipeID = id;
    pipes[freeIndex].readSemID = readSemaphore ;
    pipes[freeIndex].writeSemID = writeSemaphore ;
    pipes[freeIndex].writePos = 0;
    pipes[freeIndex].readPos = 0;
    pipes[freeIndex].qty = 0;
    pipes[freeIndex].eof = 0;

    totalPipes++;
    return DONE;
}

int getPipeIndex(unsigned int id){
	for(int i = 0; i < MAX_PIPES; i++){
		if(pipes[i].pipeID == id){
			return i;
		}
	}
	return INVALID_PIPE_ID;
}

int setupAvailablePipe() {
    int id = generateAvailablePipeID();
    if (id == NO_SPACE_ERROR) {
        return NO_SPACE_ERROR;
    }
    return (initializePipe(id) == DONE) ? id : NO_SPACE_ERROR;
}

int readPipeData(unsigned int id, char * buffer, unsigned int count){
	int pos = getPipeIndex(id);
	if(pos == INVALID_PIPE_ID){
		return INVALID_PIPE_ID;
	}
	if(pipes[pos].eof && pipes[pos].qty == 0){
		return EOF;
	}
	int i = 0;	
	for(; i < count && !(pipes[pos].eof && pipes[pos].qty == 0); i++){
		waitForSemaphore(pipes[pos].readSemID);

		buffer[i] = pipes[pos].pipe[pipes[pos].readPos];
		MOD_INCREMENT(pipes[pos].readPos, PIPE_SIZE);
		pipes[pos].qty--;

		signalSemaphore(pipes[pos].writeSemID);
	}
	return i;
}

void removePipe(unsigned int id){
	int pos = getPipeIndex(id);
	if(pos == INVALID_PIPE_ID){
		return;
	}
	removeSemaphore(pipes[pos].writeSemID);	
	removeSemaphore(pipes[pos].readSemID);
	m_free(pipes[pos].pipe);

	pipes[pos].readSemID  = 0;
	pipes[pos].writeSemID  = 0;
	pipes[pos].pipeID = 0;
	pipes[pos].writePos = 0;
	pipes[pos].readPos = 0;
	pipes[pos].qty = 0;
	pipes[pos].pipe = 0;

	totalPipes --;
}

void setPipeEOF(unsigned int id){
	int pos = getPipeIndex(id);
	if(pos == INVALID_PIPE_ID){
		return;
	}
	pipes[pos].eof = 1;
}

uint64_t getPipeDetails(pipesInfo * info){
	int activePipeCount  = 0;
	for(int i = 0; i < MAX_PIPES; i++){
		if(pipes[i].pipeID != 0){
			info[activePipeCount ].id = pipes[i].pipeID;
			info[activePipeCount ].usage = pipes[i].qty;
			info[activePipeCount ].readNumBlocked = getSemaphoreBlockedProcessByID(pipes[i].readSemID, info[activePipeCount ].readBlockedPIDS);
			info[activePipeCount ].writeNumBlocked = getSemaphoreBlockedProcessByID(pipes[i].writeSemID, info[activePipeCount ].writeBlockedPIDS);
			activePipeCount ++;
		}
	}
	return activePipeCount ;
}

int writePipeData(unsigned int id, const char * buffer, unsigned int count){
	int pos = getPipeIndex(id);
	if(pos == INVALID_PIPE_ID)
		return INVALID_PIPE_ID;

	for(int i=0; i<count; i++){
		waitForSemaphore(pipes[pos].writeSemID);
		pipes[pos].pipe[pipes[pos].writePos] = buffer[i];
		MOD_INCREMENT(pipes[pos].writePos, PIPE_SIZE);
		pipes[pos].qty++;

		signalSemaphore(pipes[pos].readSemID);
	}
	return count;
}

