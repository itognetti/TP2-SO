#include "../include/syscalls.h"
#include "../include/testUtils.h"
#include "../include/shell.h"
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 128

// Structure for memory requests
typedef struct memoryRequest {
  void *address;
  uint32_t size;
} memoryRequest;

void testMemoryManager(){
  memoryRequest memoryRequests[MAX_BLOCKS];
  uint8_t requestCount;
  uint32_t totalAllocated;
  uint64_t maxMemorySize = 2000;

  while (1) {
    requestCount = 0;
    totalAllocated = 0;

    printf("testMemoryManager: allocating memory, please wait...\n");

    // Request as many blocks as possible
    while (requestCount < MAX_BLOCKS && totalAllocated < maxMemorySize) {
      memoryRequests[requestCount].size = generateUniformRandom(maxMemorySize - totalAllocated - 1) + 1;
      memoryRequests[requestCount].address = (void *) alloc(memoryRequests[requestCount].size);

      if(memoryRequests[requestCount].address == 0){
        printf("testMemoryManager: out of memory\n");
        return ;
      }

      if(memoryRequests[requestCount].address) {
        totalAllocated += memoryRequests[requestCount].size;
        requestCount++;
      }
    }

    // Initialize memory
    for (uint32_t i = 0; i < requestCount; i++) {
      if (memoryRequests[i].address) {
        memset(memoryRequests[i].address, i, memoryRequests[i].size);
      }
    }

    // Verify memory
    for (uint32_t i = 0; i < requestCount; i++) {
      if (memoryRequests[i].address) {
        if (!checkMemory(memoryRequests[i].address, i, memoryRequests[i].size)) {
          printf("testMemoryManager: error\n");
          return ;
        }
      }
    }

    // Free memory
    for (uint32_t i = 0; i < requestCount; i++) {
      if (memoryRequests[i].address) {
        freeMem(memoryRequests[i].address);
      }
    }

    printf("testMemoryManager: OK\n");
  }
}