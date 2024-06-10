// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef USE_BUDDY
#include "../include/memoryManager.h"
#include "../include/mm_buddy.h"


static TNode *buddyTree = (TNode *) OFFSET_PTR(HP_ST, sizeof(memStatus));
static void *userMemoryStart;
static unsigned int maxClassSize;
static uint64_t buddyMemorySize;

static inline unsigned int getParentIndex(unsigned int index) {
    return ((index + 1) >> 1) - 1;
}

static inline unsigned int getLeftChildIndex(unsigned int index) {
    return (index << 1) + 1;
}

static inline unsigned int getRightChildIndex(unsigned int index) {
    return (index << 1) + 2;
}

static inline unsigned int getBuddyIndex(unsigned int index) {
    return (index % 2) == 0? index - 1 : index + 1; 
}

static inline unsigned int getFirstIndexOfClass(unsigned int classSize) {
    return POW_2(maxClassSize - classSize) - 1;
}

static unsigned int getClassSize(uint64_t size) {
    unsigned int result = 1; 

    while ((size >>= 1) != 0)
        result++;

    return result < MIN_SIZE_CLASS ? MIN_SIZE_CLASS : result;
}

void m_init() {
    maxClassSize = getClassSize(HP_SI);
    int totalNodes = POW_2((maxClassSize - MIN_SIZE_CLASS) + 1) - 1; 

    userMemoryStart = (void *) OFFSET_PTR(buddyTree, totalNodes * sizeof(TNode));
    buddyMemorySize = (uint64_t) (HP_E - userMemoryStart);

    memStatus* status = m_status();
    uint64_t free = (uint64_t) (HP_E - userMemoryStart);
    status->freeBytes = free;
}

static int findSmallestAvailableRecursively(unsigned int classSize, unsigned int currentClassSize, unsigned int index) {
    if (buddyTree[index].isAllocated)
        return -1;

    if (classSize == currentClassSize) {
        if (buddyTree[index].isSplit)
            return -1;

        buddyTree[index].isAllocated = TRUE;
        return index;
    }

    if(classSize < currentClassSize){
        int result = findSmallestAvailableRecursively(classSize, currentClassSize - 1, getLeftChildIndex(index));
        if(result == -1){
            result = findSmallestAvailableRecursively(classSize, currentClassSize - 1, getRightChildIndex(index));
        }
        if(result != -1)
            buddyTree[index].isSplit = TRUE;
        return result;
    }
    
    return -1;
}

int findSmallestAvailable(unsigned int classSize) {
    return findSmallestAvailableRecursively(classSize, maxClassSize, 0);
}

static inline void *indexToPointer(unsigned int index, unsigned int classSize) {
    return (void *) OFFSET_PTR(userMemoryStart, (index - getFirstIndexOfClass(classSize)) * (1 << classSize));
}

static inline unsigned int pointerToIndex(void *pointer) {
    return *(uint64_t *) pointer;
}

memStatus *m_status() {
    return (memStatus*) HP_ST;
}

void *m_malloc(uint64_t size) {
    if (size <= 0 || size + HEADER_SIZE > buddyMemorySize)
        return NULL;    

    unsigned int classSize = getClassSize(size + HEADER_SIZE);
    if (classSize > maxClassSize)
        return NULL;

    int index = findSmallestAvailable(classSize);
    if (index == -1)
        return NULL;

    memStatus *status = m_status();
    uint64_t blockSize = POW_2(classSize);
    status->allocatedBytes += blockSize;
    status->freeBytes -= blockSize;
    status->allocatedBlocks++;

    void *result = indexToPointer(index, classSize);
    *((uint64_t *) result) = index;

    return (void *) OFFSET_PTR(result, HEADER_SIZE);
}

static void freeMemoryRecursively(unsigned int index) {
    buddyTree[index].isAllocated = FALSE;

    int buddyIndex = getBuddyIndex(index);
    if (!buddyTree[buddyIndex].isAllocated && !buddyTree[buddyIndex].isSplit) {
        int parentIndex = getParentIndex(index);
        buddyTree[parentIndex].isSplit = FALSE;
        freeMemoryRecursively(parentIndex);
    }
}

int getClassIndex(unsigned int index) {
    for (int i = maxClassSize; i >= MIN_SIZE_CLASS; i--) {
        int firstClassIndex = getFirstIndexOfClass(i);
        int classIndexCount = getFirstIndexOfClass(i - 1) - firstClassIndex;
        if (index >= firstClassIndex && index < firstClassIndex + classIndexCount)
            return i;
    }
    return -1;
}

void m_free(void *pointer) {
    if (pointer == NULL || pointer < userMemoryStart || pointer > HP_E)
        return;

    void *realPointer = (void *) OFFSET_PTR(pointer, -HEADER_SIZE);
    unsigned int index = pointerToIndex(realPointer);
    if (!buddyTree[index].isAllocated)
        return;

    freeMemoryRecursively(index);

    uint64_t size = POW_2(getClassIndex(index));

    memStatus *status = m_status();
    status->allocatedBytes -= size;
    status->freeBytes += size;
    status->allocatedBlocks--;
}

#endif