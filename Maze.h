
#ifndef MAZE_h
#define	MAZE_h

#include "Main.h"
#include "irobot.h"
#include "ADC.h"


char x = 1,y = 2;
char coordinate[5][4];
char mark[5][4];
char victimCounter = 0;
bit terminate = 0;
int currentAngle = 0;

void chooseRouteT(void);
int convertAngle(int);
void mazeInitialise(void);
void setWall(int);
char getWall(int);
void setMark(int);
char getMark(int);
void setCoordinate(int);

char getWall1(int);
char getWall2(int);
char getWall3(int);
void angleDirection(int angle);
void travelBack(void);
void backTrack(void);

#endif

