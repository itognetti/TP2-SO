#include "./include/syscalls.h"
#include "./include/testUtils.h"
#include "./include/shell.h"

enum ProcessState { RUNNING, BLOCKED, KILLED };

typedef struct ProcessRequest {
  int32_t processId;
  enum ProcessState state;
} ProcessRequest;

uint64_t testProcesses(uint64_t argc, char *argv[]) {
  uint8_t requestIndex;
  uint8_t aliveCount = 0;
  uint8_t action;
  uint64_t maxProcesses;
  char *emptyArguments[] = {0};

  if (argc != 1)
    return -1;

  if ((maxProcesses = satoi(argv[0])) <= 0)
    return -1;

  ProcessRequest processRequests[maxProcesses];

  while (1) {
    for (requestIndex = 0; requestIndex < maxProcesses; requestIndex++) {
      processRequests[requestIndex].processId = registerChildProcess((uint64_t) &endlessLoop, 1, 1, (uint64_t) emptyArguments);
      if (processRequests[requestIndex].processId == -1) {
        printf("testProcesses: error while creating a process\n");
        return -1;
      } else {
        processRequests[requestIndex].state = RUNNING;
        aliveCount++;
      }
    }

    while (aliveCount > 0) {
      for (requestIndex = 0; requestIndex < maxProcesses; requestIndex++) {
        action = generateUniformRandom(100) % 2;

        switch (action) {
          case 0:
            if (processRequests[requestIndex].state == RUNNING || processRequests[requestIndex].state == BLOCKED) {
              if(killProcess(processRequests[requestIndex].processId) == -1){
                printf("testProcesses: error while killing a process\n");
                return -1;
              }
              processRequests[requestIndex].state = KILLED;
              aliveCount--;
            }
            break;

          case 1:
            if (processRequests[requestIndex].state == RUNNING) {
                if(pauseOrUnpauseProcess(processRequests[requestIndex].processId) == -1){
                  printf("testProcesses: error while blocking a process\n");
                  return -1;    
                }
              processRequests[requestIndex].state = BLOCKED;
            }
            break;
        }
      }

      // Desbloquea procesos aleatoriamente
      for (requestIndex = 0; requestIndex < maxProcesses; requestIndex++)
        if (processRequests[requestIndex].state == BLOCKED && generateUniformRandom(100) % 2) {
          if(pauseOrUnpauseProcess(processRequests[requestIndex].processId) == -1){
            printf("testProcesses: error while unblocking a process\n");
            return -1;
          }
          processRequests[requestIndex].state = RUNNING;
        }
    }
  }
}