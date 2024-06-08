#include <childProcess.h>

static waitData processData[MAX_WAIT_TASKS] = {{0}};

uint8_t childProcessesCompleted(unsigned int pid) {
    for (waitData *data = processData; data < processData + MAX_WAIT_TASKS; data++) {
        if (data->state == RUNNING && data->fatherPid == pid) {
            return 0;
        }
    }
    return 1;
}

uint8_t hasChildProcesses(unsigned int pid){
  for (waitData *data = processData; data < processData + MAX_WAIT_TASKS; data++) {
        if (data->fatherPid == pid) {
            return 1;
        }
    }
    return 0;
}


void removeChildProcess(unsigned int pid) {
    for (waitData *data = processData; data < processData + MAX_WAIT_TASKS; data++) {
        if (data->fatherPid == pid) {
            data->childPid = 0;
            data->fatherPid = 0;
            data->state = NOT_TRACKING;
        }
    }
}

void reportChildProcessFinished(unsigned int pid) {
    for (waitData *data = processData; data < processData + MAX_WAIT_TASKS; data++) {
        if (data->state == RUNNING && data->childPid == pid) {
            data->state = FINISHED;
            unsigned int fatherPid = data->fatherPid;
            if (childProcessesCompleted(fatherPid)) {
                removeChildProcess(fatherPid);
                changeState(fatherPid, ACTIVE_PROCESS);
            }
            return;
        }
    }
}

void waitForChildProcess() {
    int pid = getCurrentPID();
    if (!hasChildProcesses(pid)) {
        return;
    }
    changeState(pid, WAITING_FOR_CHILD);
    forceChangeTask();
}

unsigned int createChildTask(uint64_t entryP, uint8_t input, uint8_t output, char ** arg0) {
    unsigned int childPid = addTask(entryP, input, output, DEFAULT_PRIORITY, KILLEABLE, arg0);
    assignChildProcess(getCurrentPID(), childPid);
    return childPid;
}

void assignChildProcess(unsigned int fatherPid, unsigned int childPid) {
    for (waitData *data = processData; data < processData + MAX_WAIT_TASKS; data++) {
        if (data->state == NOT_TRACKING) {
            data->childPid = childPid;
            data->fatherPid = fatherPid;
            data->state = RUNNING;
            return;
        }
    }
}