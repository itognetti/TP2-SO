#include "../include/syscalls.h"

void sys_write_to_stdout(char *buffer, int row, int col, int color){    // while coding
    //sys_write(STDOUT, buffer, row, col, color);
    sys_write(getCurrentOutput(),buffer,row,col,color);

}

void sys_write(int fd, char * string, int row, int col, int color){
    switch (fd){
        case BACKGROUND:
            break;
        case STDERR:
            if(row == -1 || col == -1){
                print(string, RED);
            }
            else {
                printIn(string, row, col, color);
            }
            break;
        case STDOUT:
            if(row == -1 || col == -1){
                print(string, color);
            }
            else {
                printIn(string, row, col, color);
            }
            break;
        default:
            writeToPipe(fd, string, strlen(string));
    }
}

void sys_read(int fd, char *buffer, int length){
    if (fd == STDIN){
        readKeyboard(buffer, length);
    }
    readFromPipe(fd, buffer, length);
}

void sys_clear(){
    clearScreen();
}

void sys_time(date currentDate){
    getDate(currentDate);
}

void sys_holder(int time){
    holder(time);
}

void sys_beep(uint32_t beepTime){
    beep(beepTime);
}

void sys_info_reg(){
    getRegistersState();
}

void sys_draw(int row, int col, int color){
    putPixel(row, col, color);
}

uint64_t sys_allocate_memory(uint64_t length) {
	return (uint64_t) m_malloc(length);
}

uint64_t sys_memory_man_status(uint64_t * buffer){
    printMemStatus(buffer);
    return 1;
}

unsigned int sys_print_memory(uint64_t pos, char * buffer) {
   	if( !(pos >= MIN_MEM_POS && pos < MAX_MEM_POS) ){
		return INVALID_MEM_POS;
    }
	uint64_t current;

	for(int i = 0, j = 0; i < MAX_MEM_READ; i++) {
		if(i != 0 && i % 4 == 0) {
			buffer[j++] = ' ';
    }
    current = *((uint8_t * ) pos + i);
    j += hexToString(current, buffer + j, BYTE_LENGTH);
	}
	return 0;
}

void sys_free(void * pointer) {
    m_free(pointer);    
}

uint64_t sys_create_pipe_available(){
    return createAvailablePipe();
}

uint64_t sys_create_pipe(unsigned int pipeId){
    return createPipe(pipeId);
}

uint64_t sys_write_to_pipe(unsigned int pipeId, const char * source, unsigned int count){
    return writeToPipe(pipeId, source, count);
}

uint64_t sys_read_from_pipe(unsigned int pipeId, char * destination, unsigned int count){
    return readFromPipe(pipeId, destination, count);
}    

uint64_t sys_get_pipe_info(pipesInfo * info){
    return getPipeInfo(info);
}

void sys_destroy_pipe(unsigned int pipeId){
    destroyPipe(pipeId);
}

uint64_t sys_create_sem(unsigned int semaphoreId, unsigned int initialValue){
    return createSemaphore(semaphoreId, initialValue);
}

uint64_t sys_create_sem_available(unsigned int initialValue){
    return makeSemaphoreAvailable(initialValue);
}

uint64_t sys_signal_sem(unsigned int semaphoreId){
    return signalSemaphore(semaphoreId);
}

uint64_t sys_wait_sem(unsigned int semaphoreId){
    return waitSemaphore(semaphoreId);
}

void sys_destroy_sem(unsigned int semaphoreId){
    destroySemaphore(semaphoreId);
}

uint64_t sys_get_semaphore_info(semaphoreInfo * info){
    return getSemaphoreInfo(info);
}

uint64_t sys_register_child_process(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0){
    return addChildrenTask(entryPoint, input, output, arg0);
}

void sys_wait_for_children(){
    waitForChildren();
}

uint64_t sys_get_pid(){
    return getCurrentPID();
}

uint64_t sys_register_process(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0){
    return addTask(entryPoint, input, output, DEFAULT_PRIORITY, MORTAL, arg0);
}

uint64_t sys_pause_and_unpause_process(unsigned int processPid){
    return pauseOrUnpauseProcess(processPid);
}

uint64_t sys_alive_process(unsigned int processId){
    return findTask(processId) != NO_TASK_FOUND;
}

uint64_t sys_info_process(processInfo * info){
    return getProcessInfo(info);
}

void sys_renounce_cpu(){
    forceChangeTask();
}

uint64_t sys_kill_process(unsigned int processPid){
    return removeTask(processPid);
}

uint64_t sys_nice(uint8_t pid, int delta){
	return changePriority(pid, delta);
}