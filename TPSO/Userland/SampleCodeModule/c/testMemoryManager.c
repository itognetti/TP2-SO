// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/syscalls.h"
#include "../include/testUtils.h"
#include "../include/shell.h"
#include "../include/loader.h"
#include <string.h>

#define MAX_MEMORY_BLOCKS 128

typedef struct MemoryRequest {
  void *address;
  uint32_t size;
} MemoryRequest;

void testMemoryManager(char *argv[]) {

  MemoryRequest memRequests[MAX_MEMORY_BLOCKS];
  uint8_t currentRequest;
  uint64_t totalAllocatedMemory;
  uint64_t maxMemoryLimit;

  if ((maxMemoryLimit = satoi(argv[1])) <= 0){
    println("Invalid argument, MB value must be a positive number.");
    return;
  }
  
  while (1) {
    currentRequest = 0;
    totalAllocatedMemory = 0;

    printf("Allocating memory...\n");

    
    while (currentRequest < MAX_MEMORY_BLOCKS && totalAllocatedMemory < maxMemoryLimit) {
      memRequests[currentRequest].size = getUniform(maxMemoryLimit - totalAllocatedMemory - 1) + 1;
      memRequests[currentRequest].address = (void *) alloc(memRequests[currentRequest].size);

      if (memRequests[currentRequest].address == 0) {
        printf("Out of memory\n");
        return;
      }

      totalAllocatedMemory += memRequests[currentRequest].size;
      currentRequest++;
    }

    
    for (uint32_t i = 0; i < currentRequest; i++){
      if (memRequests[i].address){
        _memset(memRequests[i].address, i, memRequests[i].size);
      }
    }

    
    for (uint32_t i = 0; i < currentRequest; i++){
      if (memRequests[i].address){
        if (!memcheck(memRequests[i].address, i, memRequests[i].size)) {
          printf("Memory check failed\n");
          return;
        }
      }
    }

    
    for (uint32_t i = 0; i < currentRequest; i++){
      if (memRequests[i].address){
        freeMem(memRequests[i].address);
      }
    }

    printf("Memory successfully managed\n");
  }
}