#include <childProcess.h>

static waitData taskList[MAX_WAIT_TASKS] = {{0}};

uint8_t hasChildren(unsigned int pid){
    for(int i = 0; i < MAX_WAIT_TASKS; i++){
        if(taskList[i].fatherPid == pid){
            return 1;
        }
    }
    return 0;
}

void addChildren(unsigned int fatherPid, unsigned int childPid){
    for(int i = 0; i < MAX_WAIT_TASKS; i++){
        if(taskList[i].state == NOT_TRACKING){
            taskList[i].childPid = childPid;
            taskList[i].fatherPid = fatherPid;
            taskList[i].state = RUNNING;
            return;
        }
    }
}

void removeChildren(unsigned int pid){
    for(int i = 0; i < MAX_WAIT_TASKS; i++){
        if(taskList[i].fatherPid == pid){
            taskList[i].childPid = 0;
            taskList[i].fatherPid = 0;
            taskList[i].state = NOT_TRACKING;
        }
    }
}

void waitForChildren(){
    int pid = getCurrentPID();
    if(!hasChildren(pid)){
        return;
    }
    changeState(pid, WAITING_FOR_CHILD);
    forceChangeTask();
}

uint8_t childrenFinished(unsigned int pid){
    for(int i = 0; i < MAX_WAIT_TASKS; i++){
        if(taskList[i].state == RUNNING && taskList[i].fatherPid == pid){
            return 0;
        }
    }
    return 1;
}

unsigned int addChildrenTask(uint64_t entryP, uint8_t input, uint8_t output, char ** arg0){
    unsigned int childPid = addTask(entryP, input, output, DEFAULT_PRIORITY, MORTAL, arg0);
    addChildren(getCurrentPID(), childPid);
    return childPid;
}

void signalFinished(unsigned int pid){
    for(int i = 0; i < MAX_WAIT_TASKS; i++){
        if(taskList[i].state == RUNNING && taskList[i].childPid == pid){
            taskList[i].state = FINISHED;
            unsigned int fatherPid = taskList[i].fatherPid;
            if(childrenFinished(fatherPid)){
                removeChildren(fatherPid);
                changeState(fatherPid, ACTIVE_PROCESS);
            }
            return;
        }
    }
}