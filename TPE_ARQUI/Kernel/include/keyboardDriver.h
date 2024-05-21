#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H

#include <videoDriver.h>
#include <registers.h>

#define BUFFER_DIM 10
#define KEY_COUNT 256

#define ESC 0x01
#define TAB 0x0F
#define BACKSPACE 0x0E
#define CTRL 0x1D
#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define ENTER 0x1C
#define ALT 0x38
#define CAPS_LOCK 0x3A
#define RELEASED_KEY 0x80
#define MAX_PRESSED_KEY 0X7F

#define IS_ALPHA(c) c >= 'a' && c <= 'z'

extern unsigned char getPressedKey();

void keyboardHandler();
void readKeyboard(char * buffer, int length);

#endif