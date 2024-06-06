#include "../include/syscalls.h"
#include "../include/testUtils.h"
#include "../include/shell.h"

enum State { RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

void testProcesses(char *argv[]){
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  int64_t maxProcesses;
  char * argvAux[] = {0};

  if ((maxProcesses = satoi(argv[1])) <= 0){
    println("Invalid total processes value");
    return;
  }

  p_rq p_rqs[maxProcesses];

  printf("Creating processes \n");

  while (1) {

    // Create maxProcesses processes
    for (rq = 0; rq < maxProcesses; rq++) {
      p_rqs[rq].pid = registerChildProcess((uint64_t)&endlessLoop, 1, 1, argvAux);

      if (p_rqs[rq].pid == -1) {
        printf("Error creating process\n");
        return;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < maxProcesses; rq++) {
        action = getUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              if (killProcess(p_rqs[rq].pid) == -1) {
                printf("Error killing process\n");
                return;
              } 
              p_rqs[rq].state = KILLED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
              if (pauseOrUnpauseProcess(p_rqs[rq].pid) == -1) {
                printf("Error blocking process\n");
                return;
              } 
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < maxProcesses; rq++)
        if (p_rqs[rq].state == BLOCKED && getUniform(100) % 2) {
          if (pauseOrUnpauseProcess(p_rqs[rq].pid) == -1) {
            printf("Error unblocking process\n");
            return;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}