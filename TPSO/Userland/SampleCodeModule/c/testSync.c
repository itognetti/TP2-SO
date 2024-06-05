#include <testSync.h>

void slowInc(int64_t *p, int64_t inc) {
  int aux;
  aux = *p;
  aux += inc;
  for(int i = 0; i < 30000 ; i++) 
    ;
  *p = aux;
}

void myProcessInc(char *argv[]) {
  int inc = 1;
  int increments = satoi(argv[1]);

  int i;
  for (i = 0; i < increments; i++) {
    if (argv[2][0] == 's') {
      waitSemaphore(SEM_ID);
    }
    slowInc(&global, inc);
    if (argv[2][0] == 's') {
      signalSemaphore(SEM_ID);
    }
  }
}

void testSync(char *argv[]) { //{n, use_sem, 0}
  int increments, processes, semUse;

  if((increments = satoi(argv[1])) <= 0){
    printf("testSynchro: Invalid increment value\n");
    return;
  }

  if((processes = satoi(argv[2])) <= 0){
    printf("testSynchro: Invalid quantity of processes value\n");
    return;
  }

  semUse = satoi(argv[3]);
  if(semUse != 0 && semUse != 1){
    printf("testSynchro: Invalid semaphore flag value\n");
    return;
  }

  if(semUse) {
    destroySemaphore(SEM_ID);
    if(registerSemaphore(SEM_ID, 1) != 0) {
      printf("testSynchro: Error opening semaphore\n");
      return;
    }
  }

  char *args[] = {"sum", argv[1], "n", NULL};

  global = 0;

  if(semUse) {
    args[2] = "s";
  }

  int i;

  println("testSynchro: Incrementing...");

  for (i = 0; i < processes; i++) {
    int error = registerChildProcess((uint64_t)&myProcessInc, STDIN, BACKGROUND, args);
    if (error <= 0 ) {
      printf("testSynchro: Error creating children");
    }
  }

  waitChildren();

  if(semUse) {
    destroySemaphore(SEM_ID);
  }

  printf("testSynchro: Global expected value was ");
  char * expectedValue = int64ToStringConverter(increments * processes);
  println(expectedValue);
  printf("testSynchro: Global final value was ");
  char * finalValue = int64ToStringConverter(global);
  println(finalValue);

  return;
}