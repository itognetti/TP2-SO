#include "./include/syscalls.h"
#include "./include/testUtils.h"
#include "./include/shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 128

// Structure for memory requests
typedef struct memoryRequest {
  void *address;
  uint32_t size;
} memoryRequest;

uint64_t testMemoryManagement(uint64_t argumentCount, char *arguments[]) {
  memoryRequest memoryRequests[MAX_BLOCKS];
  uint8_t requestCount;
  uint32_t totalAllocated;
  uint64_t maxMemorySize;

  // Verify the number of arguments
  if (argumentCount != 1) {
    return -1;
  }

  // Convert the argument to an integer
  if ((maxMemorySize = satoi(arguments[0])) <= 0) {
    return -1;
  }

  while (1) {
    requestCount = 0;
    totalAllocated = 0;

    printf("testMm: allocating memory, please wait...\n");

    // Request as many blocks as possible
    while (requestCount < MAX_BLOCKS && totalAllocated < maxMemorySize) {
      memoryRequests[requestCount].size = generateUniformRandom(maxMemorySize - totalAllocated - 1) + 1;
      // memoryRequests[requestCount].address = malloc(memoryRequests[requestCount].size); // Need to implement syscall for malloc

      if (memoryRequests[requestCount].address) {
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
          printf("testMm: error\n");
          return -1;
        }
      }
    }

    // Free memory
    for (uint32_t i = 0; i < requestCount; i++) {
      if (memoryRequests[i].address) {
        // Need to implement syscall for free
        // free(memoryRequests[i].address);
      }
    }

    printf("testMm: OK\n");
  }
}