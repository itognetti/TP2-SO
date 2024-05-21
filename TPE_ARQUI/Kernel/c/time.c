#include <time.h>

static unsigned long ticks = 0;

void timerHandler() {
	ticks++;
}

int ticksElapsed() {
	return ticks;
}

int secondsElapsed() {
	return ticks / 18;
}

void timer_wait(uint16_t ticks) {
    for (int i = 0; i < ticks; i++){
        _hlt();
    }
}

void holder(int num) {
	int initialTicks = ticksElapsed();
	while(initialTicks < num ){
		num--;
	}
}