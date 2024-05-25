#include "../include/syscalls.h"

void sys_write(char * string, int row, int col, int color){//no las modifique ya estaban de antes
    if(row == -1 || col == -1){
        print(string, color);
    }
    else{
        printIn(string, row, col, color);
    }
}

void sys_read(char *buffer, int length){
    readKeyboard(buffer, length);
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

void sys_allocate_memory(uint64_t length) {
    (uint64_t) m_malloc(length);
}

uint64_t sysChangeProcessPriority(uint8_t processId, int priorityChange){
    return changePriority(processId, priorityChange);
}

void sys_destroy_pipe(unsigned int pipeId){
    destroyPipe(pipeId);
}

void sys_destroy_sem(unsigned int semaphoreId){
    destroySemaphore(semaphoreId);
}

uint64_t sysFreeMemory(void * pointer) {
    m_free(pointer);
    return 1;    
}

void sys_get_pid(){
    getCurrentPid();
}

uint64_t sysTerminateProcess(unsigned int processId) {
    return RemoveTask(processId);
}

uint64_t sysGetMemoryStatus(uint64_t * buffer){
    printMemStatus(buffer);
    return 1;
}

uint64_t sysPauseOrUnpauseProcess(unsigned int processId) {
    return pauseOrUnpauseProcess(processId);
}

void sys_get_pipe_info(pipesInfo * info){
    getPipeInfo(info);
}

uint64_t sysIsProcessAlive(unsigned int processId){
    return findTask(processId) != NO_TASK_FOUND;
}

uint64_t sysGetProcessInfo(processInfo * info){
    return getProcessInfo(info);
}

void sys_read_from_pipe(unsigned int pipeId, char * destination, unsigned int count){
    readFromPipe(pipeId, destination, count);
}

void sys_register_child_process(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0){
    addChildrenTask(entryPoint, input, output, arg0);
}

void sys_create_pipe_available(){
    createPipeAvailable();
}

void sys_create_pipe(unsigned int pipeId){
    createPipe(pipeId);
}

uint64_t sysRegisterProcess(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0){
    return addTask(entryPoint, input, output, DEFAULT_PRIORITY, MORTAL, arg0);
}

void sys_create_sem(unsigned int semaphoreId, unsigned int initialValue){
    createSemaphore(semaphoreId, initialValue);
}

void sys_create_sem_available(unsigned int initialValue){
    makeSemaphoreAvailable(initialValue);
}

uint64_t sysRelinquishCpu(){
    forceChangeTask();
    return 1;
}

uint64_t sysGetSemaphoreInfo(semaphoreInfo * info){
    return getSemaphoreInfo(info);
}

void sys_signal_sem(unsigned int semaphoreId){
    signalSemaphore(semaphoreId);
}

void sys_wait_for_children(){
    waitForChildren();
}

void sys_wait_sem(unsigned int semaphoreId){
    waitSemaphore(semaphoreId);
}

void sys_write_to_pipe(unsigned int pipeId, const char * source, unsigned int count){
    writeToPipe(pipeId, source, count);
}