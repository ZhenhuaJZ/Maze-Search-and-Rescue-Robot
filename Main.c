/*
 * File:   Main.c
 * Author: Jimzh
 *
 * Created on 22 April 2017, 11:13 AM
 */

#include "ser.h"
#include "spi.h"
#include "Main.h"
#include "irobot.h"
#include "ADC.h"
#include "lcdBase.h"
#include "Maze.h"
#include <xc.h>

// Interrupt service routine
// push button debounced

void interrupt isr(void) {
    //Timer 1
    if (TMR0IF) {
        TMR0 = TMR0_VAL;
        TMR0IF = 0;
        if (DS) {
            DSDebounceCount++; // Increase debounce count
            if ((DSDebounceCount > DEBOUNCE_REQ_COUNT) && (DSReleased == 1)) { //the debounce count is higher than the required count and if the pushbutton's state is released
                DSPressed = 1; // Raise the pressed flag
                DSReleased = 0; // Clear the released flag
            }
        } else {
            DSDebounceCount = 0; // Reset the debounce count
            DSReleased = 1; // raise the released flag to signify that the button hasn't been pressed
        }
    }
}

void main(void) {
    ADCsetup(); //initialize IR sensor, data is right justifed 
    setupLCD(); //Initialize LCD
    setupSPI(); //Initialize SPI to control stepper motor
    ser_init(); //Irobot communication setup
    irobot_init(); //Start irobot, full mode
    mazeInitialise(); // Initial all wall and mark condition to 0
    store_song();

    char steps = 0;
    int frontDist = 0;
    lcdSetCursor(0x00);
    lcdWriteString("Ready"); // give ready signal
    __delay_ms(100);

    while (!DSPressed); // Start program
    DSPressed = 0;
    lcdSetCursor(0x40);
    lcdWriteString("Starting Program"); // Indicating program starts
    play_song(0);
    __delay_ms(10);

    spi_transfer(cw); //Set initial IR sensor position to 45 degree
    while (steps < 80) {
        steps++;
        SM_STEP();
        __delay_ms(1.5);
    }
    spi_transfer(stopbit);

    setMark(currentAngle);

    while (!terminate) {
        /********************************************************testing programs**************************************************************/\
//                int data;
        //                lcdSetCursor(0x00); //victim testing
        //                lcdWriteString(" V ");
        //                data = getVictim();
        //                lcdWriteToDigitBCD(data);
        //
        //                play_song(0); // play song number 1
        //                __delay_ms(10000);
        //        scan();
        ////    Angle testing program
        //                drive_angle(90,1);
        //                __delay_ms(5000);
        //          rotateccw(); // rotate clockwise

        //            lcdSetCursor(0x48);
        //            lcdWriteString("Ang ");
        //            lcdWriteToDigitBCD(get_angle());
        //            __delay_ms(1400);
        /************************************************Main program*************************************************************/

        travelStraight(); //Travel Straight

        // Display coordinate and wall information
//        lcdWriteControl(0b00000001);
//        lcdSetCursor(0x40);
//        lcdWriteString(" x ");
//        lcdWriteToDigitBCD(x);
//        lcdWriteString(" y ");
//        lcdWriteToDigitBCD(y);
//        lcdSetCursor(0x00);
//        lcdWriteString(" W ");
//        lcdWriteToDigitBCD(coordinate[x][y]);


        scan(); // Scan for wall and set wall bit in coordinate[x][y]

        // Display coordinate and wall information
//        lcdWriteControl(0b00000001);
//        lcdSetCursor(0x40);
//        lcdWriteString("x ");
//        lcdWriteToDigitBCD(x);
//        lcdWriteString(" y ");
//        lcdWriteToDigitBCD(y);
//        lcdSetCursor(0x00);
//        lcdWriteString(" W ");
//        lcdWriteToDigitBCD(coordinate[x][y]);

        /*******************************front wall adjusting*******************************/
        steps = 0;
        spi_transfer(0b00001111);
        __delay_ms(10);
        spi_transfer(0b00001111);
        steps = 0;
        while (steps < 105) {
            steps++;
            SM_STEP();
            __delay_ms(1.7);
        }

        infoDisplay();
        if (distance < 700) {
            frontDist = distance - 460;
            drive_distance(frontDist);
        }

        steps = 0;
        spi_transfer(0b00001101);
        __delay_ms(10);
        spi_transfer(0b00001101);
        steps = 0;
        while (steps < 105) {
            steps++;
            SM_STEP();
            __delay_ms(1.7);
        }
        /**********************************Scan for victim****************************/
        if ((!(coordinate[x][y] & 0b00010000)) && getVictim() != 255) { // If victim was not previously detected and victim is detecting
            victimCounter += 1;
            coordinate[x][y] |= 0b00010000;
            if (victimCounter == 1)
                play_song(4); //play song
            else if (victimCounter == 2)
                play_song(3);   
        }
        /*****************************Route choosing section****************************/
        if (victimCounter == 2) {// both victim found  
            if (x == 1 && y == 2) { // Back to starting point
                stop();
                play_song(1);
                break;
            } else { // not back to starting point
//                travelBack();
                backTrack(); // Track back to starting point
            }
        } else { // both victim not found
            chooseRouteT(); // choose the next path
            setMark(currentAngle); // Set mark for next path
        }

        /*****************************choose left or right wall to follow*******************/
        steps = 0;
        infoDisplay();
        spi_transfer(0b00001111);
        if (distance < 650) { // if right side there is wall, face IR sensor that way
            p = 0;
            spi_transfer(0b00001111);
            __delay_ms(10);
            while (steps < 25) {
                steps++;
                SM_STEP();
                __delay_ms(1.7);
            }
        } else { // else face left wall 
            p = 1;
            spi_transfer(0b00001111);
            __delay_ms(10);
            while (steps < 185) {
                steps++;
                SM_STEP();
                __delay_ms(1.7);
            }
        }
        //        // Display coordinate and wall information
        lcdWriteControl(0b00000001);
        lcdSetCursor(0x40);
        lcdWriteString("x ");
        lcdWriteToDigitBCD(x);
        lcdWriteString(" y ");
        lcdWriteToDigitBCD(y);
        lcdSetCursor(0x00);
        lcdWriteString(" W ");
        lcdWriteToDigitBCD(coordinate[x][y]);
        lcdWriteString(" C ");
        lcdWriteToDigitBCD(currentAngle);
//        __delay_ms(20);
    }
}