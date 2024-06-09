// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef USE_BUDDY
#include "../include/memoryManager.h"

static memStatus mem_st={0};

void m_init(){
    mem_st.freeBytes = HP_SI; 
    addEOL((header_t *)HP_ST);
}

void * m_malloc(uint64_t length){
    if(length<=0){
        return NULL; 
    }
    uint64_t roundedSize = MASK_LAST_BIT(length+1+HEADER_SIZE);
    header_t *  freeBlock =findFree(roundedSize);
    if (freeBlock == NULL) {
        return NULL;
    }
    addBlock(freeBlock, roundedSize);
    return (void *) OFFSET_PTR(freeBlock, HEADER_SIZE);

}

void m_free(void * block) {
    if(block == NULL || block < HP_ST || block >= HP_E) {
        return;
    }

    header_t * header = (header_t *) OFFSET_PTR(block, -HEADER_SIZE);

    uint64_t blockSize = GET_SIZE(header->size);
    mem_st.allocatedBytes -= blockSize;
    mem_st.freeBytes += blockSize;
    mem_st.allocatedBlocks -= 1;

    freeBlock(header);
}

memStatus * m_status(){
    return &mem_st; 
}

void freeBlock(header_t * block) {
    block->allocated = FALSE;

    header_t * nextBlock = (header_t *) OFFSET_PTR(block, block->size);
    if(!nextBlock->allocated) {
        block->size += nextBlock->size;
    }
}

header_t * findFree(uint64_t length) {
     header_t * currentBlock = (header_t *) HP_ST;

     while(!IS_EOL(currentBlock->size) && (currentBlock->allocated || GET_SIZE(currentBlock->size) < length)) {
         currentBlock = (header_t *) OFFSET_PTR(currentBlock, GET_SIZE(currentBlock->size));
     }

     if(IS_EOL(currentBlock->size)) {
         if(OFFSET_PTR(currentBlock, length + EOL_SIZE) > (uint64_t) HP_E) {
            return NULL;
         }
     }
     return currentBlock;
}

void addEOL(header_t * block) {
    block->size = 0;
    block->allocated = TRUE;
 }

void addBlock(header_t *block, uint64_t newSize) {
    uint64_t originalSize = GET_SIZE(block->size);

    if (IS_EOL(block->size)) {
        // Add EOL marker after the new block
        addEOL((header_t *) OFFSET_PTR(block, newSize));

        block->size = newSize;
        block->allocated = TRUE;

        mem_st.allocatedBytes += newSize;
        mem_st.freeBytes -= newSize;

    } else if (newSize + MIN_REQUEST < originalSize) {  // Split the block
        block->size = newSize;
        block->allocated = TRUE;

        mem_st.allocatedBytes += newSize;
        mem_st.freeBytes -= newSize;

        block = (header_t *) OFFSET_PTR(block, newSize);

        block->size = originalSize - newSize;
        block->allocated = FALSE;

    } else {  // Use the entire block
        block->size = originalSize;
        block->allocated = TRUE;

        mem_st.allocatedBytes += originalSize;
        mem_st.freeBytes -= originalSize;
    }
    mem_st.allocatedBlocks += 1;
}

#endif