#include "../include/syscalls.h"
//#include "../include/multitasking.h"
#include "../include/memoryManager.h"
#include "../include/pipes.h"
#include "../include/semaphore.h"

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

uint64_t sysAllocateMemory(uint64_t length) {
    return (uint64_t) mmMalloc(length);
}

uint64_t sysChangeProcessPriority(uint8_t processId, int priorityChange){
    return changePriority(processId, priorityChange);
}

uint64_t sysDestroyPipe(unsigned int pipeId){
    destroyPipe(pipeId);
    return 1;
}

uint64_t sysDestroySemaphore(unsigned int semaphoreId){
    destroySem(semaphoreId);
    return 1;
}

uint64_t sysFreeMemory(void * pointer) {
    mmFree(pointer);
    return 1;    
}

uint64_t sysGetCurrentProcessId(){
    return getCurrentPid();
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

uint64_t sysGetPipeInfo(pipesInfo * info){
    return getPipeInfo(info);
}

uint64_t sysIsProcessAlive(unsigned int processId){
    return findTask(processId) != NO_TASK_FOUND;
}

uint64_t sysGetProcessInfo(processInfo * info){
    return getProcessInfo(info);
}

uint64_t sysReadFromPipe(unsigned int pipeId, char * destination, unsigned int count){
    return readFromPipe(pipeId, destination, count);
}

uint64_t sysRegisterChildProcess(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0){
    return addChildTask(entryPoint, input, output, arg0);
}

uint64_t sysCreatePipe(){
    return createPipeAvailable();
}

uint64_t sysRegisterProcess(uint64_t entryPoint, uint8_t input, uint8_t output, char ** arg0){
    return addTask(entryPoint, input, output, DEFAULT_PRIORITY, MORTAL, arg0);
}

uint64_t sysCreateSemaphore(unsigned int semaphoreId, unsigned int initialValue){
    return createSem(semaphoreId, initialValue);
}

uint64_t sysCreateSemaphoreWithInitialValue(unsigned int initialValue){
    return createSemAvailable(initialValue);
}

uint64_t sysRelinquishCpu(){
    forceChangeTask();
    return 1;
}

uint64_t sysGetSemaphoreInfo(semaphoreInfo * info){
    return getSemaphoreInfo(info);
}

uint64_t sysSignalSemaphore(unsigned int semaphoreId){
    return signalSem(semaphoreId);
}

uint64_t sys_WaitForChildProcesses(){
    waitForChildren();
    return 1;
}

uint64_t sys_WaitSemaphore(unsigned int semaphoreId){
    return waitSem(semaphoreId);
}

uint64_t sysWriteToPipe(unsigned int pipeId, const char * source, unsigned int count){
    return writeToPipe(pipeId, source, count);
}