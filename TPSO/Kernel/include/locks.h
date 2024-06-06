#ifndef LOCKS_H_
#define LOCKS_H_

void lock(unsigned int * lock);
void unlock(unsigned int * lock);
int _xadd(unsigned int * lock, int value);
int _xchg(unsigned int * lock, int value);

#endif