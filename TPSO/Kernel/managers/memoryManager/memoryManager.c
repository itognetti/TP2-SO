#ifndef USE_BUDDY
#include "../include/memoryManager.h"
#include "../include/mm_impl.h"
#include <stddef.h>

static memStatus mem_st={0};

memStatus  * get_st(){
    return &mem_st; 
}

void mem_init(){
    mem_st.freeBytes = HP_SI; 
    addEOL((header_t *)HP_ST);
}

void * mem_malloc(uint64_t lenght){
    if(lenght<=0){
        return NULL; 
    }
    uint64_t roundedSize = MASK_LAST_BIT(lenght+1+HEADER_SIZE);
    header_t *  freeBlock =findFree(roundedSize);
    if (freeBlock == NULL) {
        return NULL;
    }
    addBlock(freeBlock, roundedSize);
    return (void *) SUM_PTR(freeBlock, HEADER_SIZE);

}

void mem_free(void * block) {
    if(block == NULL || block < HP_ST || block >= HP_E) {
        return;
    }

    header_t * header = (header_t *) SUM_PTR(block, -HEADER_SIZE);

    uint64_t blockSize = GET_SIZE(header->size);
    mem_st.allocatedBytes -= blockSize;
    mem_st.freeBytes += blockSize;
    mem_st.allocatedBlocks -= 1;

    freeBlock(header);
}

memStatus  * get_st(){
    return &mem_st; 
}


void free_Block(header_t * block) {
    block->allocated = FALSE;

    header_t * nextBlock = (header_t *) SUM_PTR(block, block->size);
    if(!nextBlock->allocated) {
        block->size += nextBlock->size;
    }
}

header_t * find_Free(uint64_t length) {
     header_t * currentBlock = (header_t *) HP_ST;

     while(!IS_EOL(currentBlock->size) && (currentBlock->allocated || GET_SIZE(currentBlock->size) < length)) {
         currentBlock = (header_t *) SUM_PTR(currentBlock, GET_SIZE(currentBlock->size));
     }

     if(IS_EOL(currentBlock->size)) {
         if(SUM_PTR(currentBlock, length + EOL_SIZE) > (uint64_t) HP_E) {
            return NULL;
         }
     }
     return currentBlock;
}



void Include_EOL(header_t * block) {
    block->size = 0;
    block->allocated = TRUE;
 }


void add_Block(header_t *block, uint64_t newSize) {
    uint64_t originalSize = GET_SIZE(block->size);

    if (IS_EOL(block->size)) {
        // Add EOL marker after the new block
        header_t *eolBlock = (header_t *) SUM_PTR(block, newSize);
        addEOL(eolBlock);

        block->size = newSize;
        block->allocated = TRUE;

        mem_st.allocatedBytes += newSize;
        mem_st.freeBytes -= newSize;
    } else if (newSize + MIN_REQUEST < originalSize) {  // Split the block
        header_t *newFreeBlock = (header_t *) SUM_PTR(block, newSize);
        block->size = newSize;
        block->allocated = TRUE;

        newFreeBlock->size = originalSize - newSize;
        newFreeBlock->allocated = FALSE;

        mem_st.allocatedBytes += newSize;
        mem_st.freeBytes -= newSize;
    } else {  // Use the entire block
        block->size = originalSize;
        block->allocated = TRUE;

        mem_st.allocatedBytes += originalSize;
        mem_st.freeBytes -= originalSize;
    }
    mem_st.allocatedBlocks += 1;
}

#endif
