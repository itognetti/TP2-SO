#ifndef USE_BUDDY   

#ifndef _MM_IMPL_H_
#define _MM_IMPL_H_

// Source:  https://www.youtube.com/watch?v=74s0m4YoHgM

#include "memoryManager.h"

typedef union header_t {
    uint64_t size;
    uint8_t allocated : 1;  // the bit_0
} header_t;

#define HEADER_SIZE (sizeof(header_t))
#define EOL_SIZE HEADER_SIZE
#define MIN_REQUEST (HEADER_SIZE + 2)
#define MASK_LAST_BIT(num) ((num) & ~0x1)
#define GET_SIZE(size) (MASK_LAST_BIT(size))
#define IS_EOL(size) (!(GET_SIZE(size) > 0))    // EOL = End Of File

void addBlock(header_t * ptr, uint64_t len);
header_t * findFree(uint64_t len);
void freeBlock(header_t * ptr);
void addEOL(header_t * ptr);

#endif
#endif