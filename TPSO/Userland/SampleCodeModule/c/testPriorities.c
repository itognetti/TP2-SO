#include <stdint.h>
#include "../include/syscalls.h"
#include "../include/testUtils.h"
#include "../include/shell.h"


#define MINOR_WAIT 1000000 
#define WAIT 50000000    

#define TOTAL_PROCESSES 3
#define LOWEST 1  
#define MEDIUM 3 
#define HIGHEST 5 

int64_t priorities[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void testPriorities(){
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = registerChildProcess((uint64_t) &endlessLoopPrint, 1, 1, (uint64_t) argv);

  bussyWait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    niceProcess(pids[i], priorities[i]);

  bussyWait(WAIT);
  printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pauseProcess(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
      niceProcess(pids[i], MEDIUM);

  printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pauseProcess(pids[i]);

  bussyWait(WAIT);
  printf("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    killProcess(pids[i]);
}