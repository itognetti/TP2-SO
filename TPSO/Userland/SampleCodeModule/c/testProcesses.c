// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/syscalls.h"
#include "../include/testUtils.h"
#include "../include/shell.h"

enum ProcessState { RUNNING,
                    BLOCKED,
                    KILLED };

typedef struct ProcessRequest {
  int32_t pid;
  enum ProcessState state;
} ProcessRequest;

void testProcesses(char *argv[]) {
  int64_t maxProcesses;
  uint8_t aliveProcesses = 0;
  char *auxiliaryArgs[] = {0};

  if ((maxProcesses = satoi(argv[1])) <= 0) {
    println("Invalid total processes value");
    return;
  }

  ProcessRequest processRequests[maxProcesses];
  printf("Creating processes\n");

  for (int i = 0; i < maxProcesses; i++) {
    processRequests[i].pid = registerChildProcess((uint64_t)&endlessLoop, 1, 1, auxiliaryArgs);

    if (processRequests[i].pid == -1) {
      printf("Error creating process\n");
      return;
    } else {
      processRequests[i].state = RUNNING;
      aliveProcesses++;
    }
  }

  while (aliveProcesses > 0) {
    for (int i = 0; i < maxProcesses; i++) {
      uint8_t action = getUniform(100) % 2;

      switch (action) {
        case 0: // Kill process
          if (processRequests[i].state == RUNNING || processRequests[i].state == BLOCKED) {
            if (killProcess(processRequests[i].pid) == -1) {
              printf("Error killing process\n");
              return;
            }
            processRequests[i].state = KILLED;
            aliveProcesses--;
          }
          break;

        case 1: 
          if (processRequests[i].state == RUNNING) {
            if (pauseOrUnpauseProcess(processRequests[i].pid) == -1) {
              printf("Error blocking process\n");
              return;
            }
            processRequests[i].state = BLOCKED;
          }
          break;
      }
    }

    
    for (int i = 0; i < maxProcesses; i++) {
      if (processRequests[i].state == BLOCKED && getUniform(100) % 2) {
        if (pauseOrUnpauseProcess(processRequests[i].pid) == -1) {
          printf("Error unblocking process\n");
          return;
        }
        processRequests[i].state = RUNNING;
      }
    }
  }
}