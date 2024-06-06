#include "../include/syscalls.h"
#include "../include/testUtils.h"
#include "../include/shell.h"
#include "../include/loader.h"
#include <string.h>

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

void testMemoryManager(char * argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint64_t total;
  uint64_t maxMemory;

  if ((maxMemory = satoi(argv[1])) <= 0){
    println("Invalid MB value");
    return;
  }
  
  while (1) {
    rq = 0;
    total = 0;

    printf("Allocating memory...\n");

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < maxMemory) {
      mm_rqs[rq].size = getUniform(maxMemory - total - 1) + 1;
      mm_rqs[rq].address = (void *) alloc(mm_rqs[rq].size);

      if(mm_rqs[rq].address == 0) {
        printf("Out of memory\n");
        return;
      }

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set memory
    uint32_t i;
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address){
        _memset(mm_rqs[i].address, i, mm_rqs[i].size);
        }
    }
    // Testing
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address){
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("Error\n");
          return;
        }
      }
    }

    // Free memory
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address){
          freeMem(mm_rqs[i].address);
      }
    printf("OK\n");
  }
}