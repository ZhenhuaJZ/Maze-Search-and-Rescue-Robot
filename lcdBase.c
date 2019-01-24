#include "lcdBase.h"
#include "ADC.h"
#include "ser.h"
#include "irobot.h"
#include "Main.h"

//functions
//write controls to LCD

void lcdWriteControl(unsigned char databyte) {
    EN = 0;
    RW = 0;
    RS = 0;
    LCD_DATA = databyte;
    EN = 1;
    EN = 0;
    __delay_ms(2);
}

//write data to LCD

void lcdWriteData(unsigned char databyte) {
    EN = 0;
    RW = 0;
    RS = 1;
    LCD_DATA = databyte;
    EN = 1;
    EN = 0;
    __delay_ms(1);
}

//Function dedicated to display both IR sensor and potential meter information

void infoDisplay(void) {
    //Display IR sensor information
        rawIrVal = getADC();
        distance = disConvert();
        lcdSetCursor(00);
        lcdWriteString("IR ");
        lcdWriteToDigitBCD(rawIrVal);
        lcdWriteString(" DST ");
        lcdWriteToDigitBCD(distance);
}

//move the LCD cursor to a particular location

void lcdSetCursor(unsigned char address) {
    address |= 0b10000000; //format address command using mask
    lcdWriteControl(address); //write address command
}

void lcdWriteString(char * s) {
    // write characters
    while (*s) lcdWriteData(*s++);
}

// convert value from 0-9999 to BCD to display on LCD

void lcdWriteToDigitBCD(unsigned int data) {
    unsigned char OnesDigit, TensDigit, Hundreds, Thousands; // Define digits

    // clear previous digit value
    Thousands = 0;
    Hundreds = 0;
    OnesDigit = 0;
    TensDigit = 0;

    //Perform a BCD Conversion
    while (data >= 1000) {
        data = data - 1000;
        ++Thousands;
    }
    while (data >= 100) {
        data = data - 100;
        ++Hundreds;
    }
    while (data >= 10) {
        data = data - 10;
        ++TensDigit;
    }
    OnesDigit = data;
    lcdWriteData((unsigned char) Thousands + 48);
    lcdWriteData((unsigned char) Hundreds + 48);
    lcdWriteData((unsigned char) TensDigit + 48);
    lcdWriteData((unsigned char) OnesDigit + 48);
}

void setupLCD(void) {
    PORTD = 0; //set all pins on portd low
    PORTE = 0; //set all pins on porte low

    TRISD = 0b00000000; //set all pins to output
    TRISE = 0b00000000; //set all pins to output

    //LCD Initialisation
    lcdWriteControl(0b00000001); //clear display
    lcdWriteControl(0b00111000); //set up display
    lcdWriteControl(0b00001100); //turn display on
    lcdWriteControl(0b00000110); //move to first digit
    lcdWriteControl(0b00000010); //entry mode setup

}

