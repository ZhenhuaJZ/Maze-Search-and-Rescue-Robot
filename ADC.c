#include "Main.h"
#include "ADC.h"

void ADCsetup(void){
    PORTA = 0x00; // Clear port A
    TRISA = 0xFF; // Make all pins in port A input
    
    ADCON0 = 0b10000001; // Initially set at channel 0, sampling at frequency of Fosc/32 and turn on ADC module
    ADCON1 = 0b10000010; // Output data is store right justified and set port A as anagloue, port e digital.
    
    __delay_us(50); // Delay for ADC aquisition
}

/*
 * disConvert converts the raw value of IR sensor to distance value in mm
 */
unsigned int disConvert(void){ 
    volatile unsigned int calResult = 0;
    if(rawIrVal <= 572 && rawIrVal >= 242)       // Distance conversion from 15 cm to 50 cm
        calResult = ((rawIrVal-710.3)/(-9.7451))*10;

    else if(rawIrVal < 242 && rawIrVal >= 171) // Distance conversion from 50 cm to 70 cm
        calResult = ((rawIrVal-417.0)/(-3.55))*10;
    
    else if(rawIrVal < 171 && rawIrVal >= 106) // Distance conversion from 70 cm to 110 cm
        calResult = ((rawIrVal-281.5)/(-1.61))*10;
    
    else if(rawIrVal < 106 && rawIrVal >= 91) // Distance conversion from 110 cm to 130 cm
        calResult = ((rawIrVal-189.0)/(-0.75))*10;
    
    else if(rawIrVal < 91 && rawIrVal >= 79) // Distance conversion from 130 cm to 150 cm
        calResult = ((rawIrVal-168.33)/(-0.6))*10;
    else
        return 9999;
    
    return (calResult);
}


/*
 * getADC returns value base on given channel argument
 * the result is right justified, ADRESL is store to adcRAW, ADRESH is store to adcRAW with 8 bit shift left
 */
unsigned int getADC(void){    
    volatile unsigned int adcRAW;

    ADRESH = 0; // Clear both registers
    ADRESL = 0;
    GO = 1; // Start ADC processing
    while(GO) continue; // Wait for conversion to complete
    
    adcRAW = ADRESL;    // Obtain 8 least significant bits of data
    adcRAW = adcRAW |= (ADRESH << 8);   // Obtain 2 most significant bit and combine by 8 bit shift left
    return (adcRAW);
}
