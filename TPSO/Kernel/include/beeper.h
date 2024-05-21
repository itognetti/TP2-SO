#ifndef BEEPER_H
#define BEEPER_H 

#include <stdint.h>
#include <videoDriver.h>
#include <colors.h>
#include <time.h>
#include <interrupts.h>

void play_sound(uint32_t nFrequence);

void beep(uint32_t beepTime);

void stopBeep();

#endif