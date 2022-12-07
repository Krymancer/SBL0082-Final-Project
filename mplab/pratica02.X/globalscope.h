#ifndef GLOBALSCOPE_H
#define	GLOBALSCOPE_H

#define true 1
#define false 0

#define ST_IDLE 0xf01
#define ST_INIT 0xf02
#define ST_WAIT 0xf03
#define ST_RESET 0xf04
#define ST_P1 0xf05 // Player 1 Press
#define ST_P2 0xf06 // Player 2 Press
#define ST_MP 0xf07 // Mediador Press

#define NO_PLAYER 0
#define PLAYER1 0xd01
#define PLAYER2 0xd02

int currentState;

int currentTime;

int firstPlayer;

int player1Pressed;
int player2Pressed;

float player1Time;
float player2Time;

int p1concorrency;
int p2concorrency;

int gameStatus;
#endif	/* GLOBALSCOPE_H */

