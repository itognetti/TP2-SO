#ifndef PONG_H
#define PONG_H

#include <libIO.h>
#include <colors.h>
#include <shell.h>
#include <syscalls.h>
#include <string.h>

#define WIDTH 1024  //x
#define HEIGHT 540  //y
//cada char es de 8x16 pixeles y la pantalla es de 1024x768 pixeles
#define LINE 16
#define LETTER 8

#define PADDLE_HEIGHT 60
#define PADDLE_WIDTH 8

#define PADDLE1_X_POSITION (5 * LETTER)
#define PADDLE2_X_POSITION (WIDTH - PADDLE_WIDTH - 5 * LETTER)

#define BALL_RADIUS 8

#define TOP_BOUNDARY (LINE * 4)
#define BOTTOM_BOUNDARY (HEIGHT - LINE - 2)

#define MOVEMENT 4

void menuPong();
void playPong();
void drawScreen();
void setup();
void movePlayer(char c);
void moveBall();
void drawObjects(int xFrom, int xTo, int yFrom, int yTo, int color);
void drawPaddle(int x, int y, int player);
void undrawPaddle(int x, int y);
void drawBall(int x, int y);
void undrawBall(int x, int y);
void resetGame();
int abs(int value);
void regenerateMiddleLine();

#endif