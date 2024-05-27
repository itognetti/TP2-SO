#ifndef LOCKS_H_
#define LOCKS_H_

void lock(unsigned int * lock);
void unlock(unsigned int * lock);

extern int _xchg(unsigned int * lock, int value);
extern int _xadd(unsigned int * lock, int value);

#endif