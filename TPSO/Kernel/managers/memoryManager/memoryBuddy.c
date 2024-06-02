#ifdef USE_BUDDY

#include "../include/memoryManager.h"
#include "../include/mm_buddy.h"

#define NULL 0

static TreeNode *buddyTree = (TreeNode *) ADD_PTR(HEAP_START, sizeof(MemoryStatus));
static void *userMemoryStart;
static unsigned int maxClassSize;
static uint64_t buddyMemorySize;

static inline unsigned int getParentIndex(unsigned int index) {
    return (index - 1) >> 1;
}

static inline unsigned int getLeftChildIndex(unsigned int index) {
    return (index << 1) + 1;
}

static inline unsigned int getRightChildIndex(unsigned int index) {
    return (index << 1) + 2;
}

static inline unsigned int getBuddyIndex(unsigned int index) {
    return index ^ 1; 
}

static inline unsigned int getFirstIndexOfClass(unsigned int classSize) {
    return (1 << (maxClassSize - classSize)) - 1;
}

static unsigned int getClassSize(uint64_t size) {
    unsigned int result = 0; 

    while (size >>= 1)
        result++;

    return result < MIN_CLASS_SIZE ? MIN_CLASS_SIZE : result + 1;
}

void memoryManagerInitialize() {
    maxClassSize = getClassSize(HEAP_SIZE);
    int totalNodes = (1 << (maxClassSize - MIN_CLASS_SIZE + 1)) - 1; 

    userMemoryStart = ADD_PTR(buddyTree, totalNodes * sizeof(TreeNode));
    buddyMemorySize = HEAP_END - userMemoryStart;

    MemoryStatus* status = getMemoryStatus();
    status->freeBytes = buddyMemorySize;
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

    int leftIndex = findSmallestAvailableRecursively(classSize, currentClassSize - 1, getLeftChildIndex(index));
    if (leftIndex != -1) {
        buddyTree[index].isSplit = TRUE;
        return leftIndex;
    }

    int rightIndex = findSmallestAvailableRecursively(classSize, currentClassSize - 1, getRightChildIndex(index));
    if (rightIndex != -1) {
        buddyTree[index].isSplit = TRUE;
        return rightIndex;
    }

    return -1;
}

int findSmallestAvailable(unsigned int classSize) {
    return findSmallestAvailableRecursively(classSize, maxClassSize, 0);
}

static inline void *indexToPointer(unsigned int index, unsigned int classSize) {
    return ADD_PTR(userMemoryStart, (index - getFirstIndexOfClass(classSize)) * (1 << classSize));
}

static inline unsigned int pointerToIndex(void *pointer) {
    return *(unsigned int *)pointer;
}

MemoryStatus *getMemoryStatus() {
    return (MemoryStatus*) HEAP_START;
}

void *allocateMemory(uint64_t size) {
    if (size <= 0 || size + HEADER_SIZE > buddyMemorySize)
        return NULL;    

    unsigned int classSize = getClassSize(size + HEADER_SIZE);
    if (classSize > maxClassSize)
        return NULL;

    int index = findSmallestAvailable(classSize);
    if (index == -1)
        return NULL;

    MemoryStatus *status = getMemoryStatus();
    uint64_t blockSize = 1 << classSize;
    status->allocatedBytes += blockSize;
    status->freeBytes -= blockSize;
    status->allocatedBlocks++;

    void *result = indexToPointer(index, classSize);
    *((unsigned int *) result) = index;

    return ADD_PTR(result, HEADER_SIZE);
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
    for (int i = maxClassSize; i >= MIN_CLASS_SIZE; i--) {
        int firstClassIndex = getFirstIndexOfClass(i);
        int classIndexCount = getFirstIndexOfClass(i - 1) - firstClassIndex;
        if (index >= firstClassIndex && index < firstClassIndex + classIndexCount)
            return i;
    }
    return -1; // should not happen
}

void freeMemory(void *pointer) {
    if (pointer == NULL || pointer < userMemoryStart || pointer > HEAP_END)
        return;

    void *realPointer = ADD_PTR(pointer, -HEADER_SIZE);
    unsigned int index = pointerToIndex(realPointer);
    if (!buddyTree[index].isAllocated)
        return;

    freeMemoryRecursively(index);

    uint64_t size = 1 << getClassIndex(index);

    MemoryStatus *status = getMemoryStatus();
    status->allocatedBytes -= size;
    status->freeBytes += size;
    status->allocatedBlocks--;
}

#endif // USE_BUDDY