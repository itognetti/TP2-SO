#include <memoryManager.h>

void printMemStatus(uint64_t buffer[3]) {
  memStatus * status = m_status();

  buffer[0] = status->allocatedBytes;
  buffer[1] = status->freeBytes;
  buffer[2] = status->allocatedBlocks;
}