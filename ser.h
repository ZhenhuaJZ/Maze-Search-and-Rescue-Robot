#ifndef _SER_H
#define _SER_H

#include "Main.h"

volatile bit processing = 0;
//initalise serial comms
void ser_init(void);	

//transmits char data through serial
void ser_putch(unsigned char c);	

//receives serial data
unsigned char ser_getch();

#endif //_SER_H