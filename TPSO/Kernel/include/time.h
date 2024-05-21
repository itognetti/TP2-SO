#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>
#include <interrupts.h>

void timerHandler();
int ticksElapsed();
int secondsElapsed();
void holder(int delta);
void timer_wait(uint16_t ticks);

#endif
