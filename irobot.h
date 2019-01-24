

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef IROBOT_H
#define	IROBOT_H


unsigned char cw = 0b00001101; //setup stepper motor to rotate clockwise
unsigned char ccw = 0b00001111; //setup stepper motor to rotate counter clockwise
unsigned char stopbit = 0x00;
bit p = 0;  // directional bit indicating current following wall, 0 is right wall follow, 1 is left wall follow
volatile bit bumpStop = 0;

int baseSpeed = 300; // base speed for robot to travel straight

int lastError = 0; // last PID error for derivative control
int cumError = 0;   // accumulative error for integration control

volatile unsigned char hByte = 0; // high byte of information send back by irobot
volatile unsigned char lByte = 0; // low byte of information send back by irobot

void irobot_init(void);

void drive(int, int);

void stop(void);

void rotatecw(void);

void rotateccw(void);

void scan(void);

signed int get_distance(void);

void drive_distance(signed int requiredDistance);

void store_song(void);

void play_song(char);

void drive_angle(int, unsigned char);

signed int get_angle(void);

signed int getData(void);

void bump(void);

void bumpstop(void);


char getVirtualWall(void);

char getVictim(void);

void rotate90cw(void);

void rotate90ccw(void);

int PID (void);

void rotate180(void);

void travelStraight(void);

#endif	/* XC_HEADER_TEMPLATE_H */

