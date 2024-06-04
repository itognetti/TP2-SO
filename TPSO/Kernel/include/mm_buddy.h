#ifdef USE_BUDDY
#ifndef _MM_BUDDY_H_
#define _MM_BUDDY_H_

#define MIN_SIZE_CLASS 6

#define HEADER_SIZE (sizeof(uint64_t)) 

#define POW_2(exp) ((uint64_t) 1 << (exp))

typedef struct TNode{
    char isAllocated;
    char isSplit;
} TNode;

int findSmallestAvailable(unsigned int sizeClass);

#endif // _MM_BUDDY_H_
#endif