 
#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> // include processor files - each processor file is guarded.  

#pragma config BOREN = OFF, CPD = OFF, WRT = OFF, FOSC = HS, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF // System configuration
#define _XTAL_FREQ 20000000 //Define system frequency as 20 Mhz
#define TMR0_VAL 100
#define LED0 RB0 // PORT B pin 0 and connect it to an LED to show if the button has been pressed or not
#define DS !RB2 // We?ll NOT the pushbutton input to convert it to active high logic
#define DEBOUNCE_REQ_COUNT 10 // Ehhh we can make this any reasonable number

volatile bit DSPressed = 0; // Bit flag to signal if the pushbutton is pressed
volatile bit DSReleased = 0; // Bit to signal if the pushbutton has been released
volatile unsigned char DSDebounceCount = 0; // Let this be the counter for seeing how many times the button is high in a row


void interrupt isr (void);
void main(void);
#endif	/* XC_HEADER_TEMPLATE_H */

