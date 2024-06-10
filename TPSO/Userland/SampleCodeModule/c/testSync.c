// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

void testSync(char *argv[]) { 
  int increments, processes, semUse;

  if((increments = satoi(argv[1])) <= 0){
    printf("Invalid argument, increment value must be a positive number\n");
    return;
  }

  if((processes = satoi(argv[2])) <= 0){
    printf("Invalid argument, processes quantity value must be a positive number.\n");
    return;
  }

  semUse = satoi(argv[3]);
  if(semUse != 0 && semUse != 1){
    printf("Invalid argument, semaphore flag value must be a number.\n");
    return;
  }

  if(semUse) {
    destroySemaphore(SEM_ID);
    if(registerSemaphore(SEM_ID, 1) != 0) {
      printf("Error opening semaphore\n");
      return;
    }
  }

  char *args[] = {"sum", argv[1], "n", NULL};

  global = 0;

  if(semUse) {
    args[2] = "s";
  }

  int i;

  println("Incrementing...");

  for (i = 0; i < processes; i++) {
    int error = registerChildProcess((uint64_t)&myProcessInc, STDIN, BACKGROUND, args);
    if (error <= 0 ) {
      printf("Error creating children");
    }
  }

  waitChildren();

  if(semUse) {
    destroySemaphore(SEM_ID);
  }

  printf("Global expected value: ");
  char * expectedValue = int64ToStringConverter(increments * processes);
  println(expectedValue);
  printf("Global final value: ");
  char * finalValue = int64ToStringConverter(global);
  println(finalValue);

  return;
}