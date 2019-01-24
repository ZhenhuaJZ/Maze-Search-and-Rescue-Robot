
#ifndef ADC_H
#define	ADC_H

volatile unsigned int rawIrVal = 0; // raw IR sensor value
volatile unsigned int distance = 0; // converted IR value in distance

void ADCsetup(void);        // Setup ADC channels, right justified, analog 3 as 3 volt reference pin
unsigned int getADC(void);
void adcDataProcess(void);

#endif

