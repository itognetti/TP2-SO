#include <testSync.h>

void slowInc(int64_t *p, int64_t inc) {
  int aux;
  aux = *p;
  aux += inc;
  for(int i = 0; i < 30000 ; i++) 
    ;
  *p = aux;
}

void myProcessInc(uint64_t argc, char *argv[]) {
  int inc = 1;

  int i;
  for (i = 0; i < MAX; i++) {
    if (argv[1][0] == 's') {
      waitSemaphore(SEM_ID);
    }
    slowInc(&global, inc);
    if (argv[1][0] == 's') {
      signalSemaphore(SEM_ID);
    }
  }
}

uint64_t testSync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
 int semUse = satoi(argv[1]);
  if(semUse) {
    destroySemaphore(SEM_ID);
    if (registerSemaphore(SEM_ID, 1) != 0) {
      printf("testSync: Error opening semaphore\n");
      return -1;
    }
  }

  char *args[] = {"sem", NULL, NULL};

  global = 0;

  if(semUse) {
    args[1] = "s";
  }

  int i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    int error = registerChildProcess((uint64_t)&myProcessInc, STDIN, BACKGROUND, (uint64_t) args);
    if (error <= 0 ) {
      printf("testSync : Error creating children");
    }
  }

  waitChildren();

  if(semUse) {
    destroySemaphore(SEM_ID);
  }

  printf("testSync: Global expected value was ");
  char * expectedValue = int64ToStringConverter(MAX * TOTAL_PAIR_PROCESSES);
  println(expectedValue);
  printf("testSync: Global final value was ");
  char * finalValue = int64ToStringConverter(global);
  println(finalValue);

  return 0;
}