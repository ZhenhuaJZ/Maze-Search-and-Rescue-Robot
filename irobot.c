#include "irobot.h"
#include "Main.h"
#include "lcdBase.h"
#include "ADC.h"
#include "ser.h"
#include "spi.h"
#include "Maze.h"
#include <stdlib.h>
// current Irobot

void irobot_init(void) {
    ser_putch(128); //setup iRobot START OPCODE
    ser_putch(132); //activate FULL MODE
}

/*----------------------------------------Sensor functions------------------------------------*/

signed int get_distance(void) {
    int data = 0;
    // get distance travelled from the packets of the irobot
    ser_putch(142);
    __delay_ms(5);
    ser_putch(19);
    __delay_ms(5);
    data = getData();
    return data;
}

signed int get_angle(void) {
    int data = 0;
    // get angle from the packet of the irobot
    ser_putch(142);
    ser_putch(20);
    data = getData();
    if (data < 0) {
        data = -data;
        return data;
    }
    return data;
}

char getVirtualWall(void) {
    char data = 0;
    ser_putch(142);
    ser_putch(13);
    data = ser_getch();
    return data;
}

char getVictim(void) {
    char data = 0;
    ser_putch(142);
    ser_putch(17);
    data = ser_getch();
    if (data == 246 || data == 254 || data == 252 || data == 250 || data == 242)
        return data;
    else
        return 255;
}

void scan(void) {// Scan front left and right wall condition
    //********************Rotate back from 68 degree wall facing either left or right
    char steps = 0;
    if (p == 0) {
        spi_transfer(0b00001111);
        __delay_ms(10);
        spi_transfer(0b00001111);
    } else {
        spi_transfer(0b00001101);
        __delay_ms(10);
        spi_transfer(0b00001101);
    }
    while (steps < 80) {
        steps++;
        SM_STEP();
        __delay_ms(1.7);
    }
    infoDisplay();
    if (distance < 700)
        setWall(currentAngle);


    //*****************************Scan left wall   
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
    if (distance < 700)
        setWall(currentAngle - 90);


    //*****************************Scan right wall
    spi_transfer(0b00001101);
    __delay_ms(10);
    spi_transfer(0b00001101);
    steps = 0;
    while (steps < 210) {
        steps++;
        SM_STEP();
        __delay_ms(1.7);
    }
    infoDisplay();
    if (distance < 700)
        setWall(currentAngle + 90);


    __delay_ms(10);
}

void store_song(void) {
    ser_putch(140);
    ser_putch(0);
    ser_putch(4);
    ser_putch(62);
    ser_putch(12);
    ser_putch(66);
    ser_putch(12);
    ser_putch(69);
    ser_putch(12);
    ser_putch(74);
    ser_putch(36);

    //game of thrones
    ser_putch(140);
    ser_putch(1);
    ser_putch(9);
    ser_putch(79);
    ser_putch(50);
    ser_putch(72);
    ser_putch(40);
    ser_putch(74);
    ser_putch(20);
    ser_putch(77);
    ser_putch(20);
    ser_putch(79);
    ser_putch(20);
    ser_putch(72);
    ser_putch(40);
    ser_putch(76);
    ser_putch(10);
    ser_putch(77);
    ser_putch(10);
    ser_putch(74);
    ser_putch(50);

    //Pokemon
    ser_putch(140);
    ser_putch(2);
    ser_putch(8);
    ser_putch(33);
    ser_putch(33);
    ser_putch(33);
    ser_putch(33);
    ser_putch(33);
    ser_putch(31);
    ser_putch(40);
    ser_putch(36);
    ser_putch(36);
    ser_putch(33);
    ser_putch(31);
    ser_putch(41);
    ser_putch(44);
    ser_putch(41);
    ser_putch(47);
    ser_putch(47);
    ser_putch(47);
    ser_putch(33);
    ser_putch(31);
    ser_putch(41);
    ser_putch(41);
    ser_putch(33);
    ser_putch(33);
    ser_putch(31);
    ser_putch(41);
    ser_putch(46);

    //Need error
    ser_putch(140);
    ser_putch(3);
    ser_putch(4);
    ser_putch(73);
    ser_putch(8);
    ser_putch(71);
    ser_putch(8);
    ser_putch(73);
    ser_putch(32);
    ser_putch(66);
    ser_putch(32);

    //song 4
    ser_putch(140);
    ser_putch(4);
    ser_putch(14);
    ser_putch(40);
    ser_putch(24);
    ser_putch(35);
    ser_putch(24);
    ser_putch(33);
    ser_putch(6);
    ser_putch(35);
    ser_putch(6);
    ser_putch(33);
    ser_putch(6);
    ser_putch(35);
    ser_putch(6);
    ser_putch(55);
    ser_putch(24);
    ser_putch(33);
    ser_putch(24);
    ser_putch(35);
    ser_putch(6);
    ser_putch(33);
    ser_putch(6);
    ser_putch(35);
    ser_putch(6);
    ser_putch(33);
    ser_putch(6);
    ser_putch(55);
    ser_putch(24);
    ser_putch(53);
    ser_putch(24);

    //song 5
    ser_putch(140);
    ser_putch(5);
    ser_putch(4);
    ser_putch(62);
    ser_putch(12);
    ser_putch(69);
    ser_putch(12);
    ser_putch(66);
    ser_putch(12);
    ser_putch(74);
    ser_putch(36);
}

void play_song(char n) {
    ser_putch(141);
    ser_putch(n);
}

signed int getData(void) { // obtain data from irobot when sensor package is send
    int data = 0;
    data = ser_getch(); // save high byte shift 8 bits
    data = (data << 8);
    data |= ser_getch(); // get low byte combine with high byte
    return data;
}

void bump(void) {// Sense if robot bumps into objects
    int data = 0;
    char wl, wr, bl, br;
    ser_putch(142);
    ser_putch(7);
    data = ser_getch();
    wl = data & 0b00001000;
    wr = data & 0b00000100;
    bl = data & 0b00000010;
    br = data & 0b00000001;
    if (bl || br)
        bumpStop = 1;
    else
        bumpStop = 0;
}

int PID(void) { // Complete PID control for traveling straight and maintain center distance
    int error = 0, pCorrection = 0, dCorrection = 0, iCorrection = 0, slope = 0, pidCorrection = 0;

    float pVal = 0.45, iVal = 0.00002, dVal = 0.07;
    error = 500 - distance; // proportional control
    pCorrection = pVal * error;

    slope = error - lastError; // derivative control
    dCorrection = slope * dVal;

    cumError = cumError + error; // integral control
    iCorrection = cumError * iVal;

    pidCorrection = pCorrection + iCorrection + dCorrection;
    lastError = error;

    return pidCorrection;
}

/*---------------------------------Movements functions------------------------------------*/
void drive(int left, int right) {
    char rh, rl, lh, ll; // convert 16bit left and right value to 8 bit low byte and high byte
    ll = left;
    lh = left >> 8;
    rl = right;
    rh = right >> 8;
    // set speed to 200mm/s forward
    ser_putch(145);
    ser_putch(rh); // Right wheel high byte
    ser_putch(rl); // Right wheel low byte
    ser_putch(lh); // left wheel high byte
    ser_putch(ll); // Left wheel low byte
}

void drive_distance(signed int requiredDistance) {
    int travelDist = 0; // Raw travel distance
    int nTravelDist = 0; // Negative travel distance
    if (requiredDistance < 0) { // if required distance is less than 0, travel backward
        drive(-300, -300); // drive backward
        while (requiredDistance < travelDist) {
            infoDisplay(); // display ADC information  
            travelDist += get_distance(); // Get total traveled distance
            nTravelDist = -travelDist; // invert to get positive distance
            __delay_ms(20);

        }
    } else { // else travel straight
        drive(300, 300); // drive straight 200mm/s
        while (requiredDistance > travelDist) {
            infoDisplay();
            travelDist += get_distance(); // get total traveled distance
            __delay_ms(20);
        }
    }
    stop();
}

void rotateccw(void) {//rotate cw on the spot
    ser_putch(145);
    ser_putch(1);
    ser_putch(44);
    ser_putch(254);
    ser_putch(212);
}

void rotatecw(void) {//rotate ccw on the spot
    ser_putch(145);
    ser_putch(254);
    ser_putch(212);
    ser_putch(1);
    ser_putch(44);
}

void rotate90cw(void) {
    int angle = 0;
    get_angle();
    currentAngle = convertAngle(currentAngle + 90);
    rotatecw();
    while (angle < 82) {
        __delay_ms(10);
        angle += get_angle();
    }
    stop();

}

void rotate90ccw(void) {

    int angle = 0;
    get_angle();
    currentAngle = convertAngle(currentAngle - 90);
    rotateccw();
    while (angle < 80) {
        __delay_ms(10);
        angle += get_angle();
    }
    stop();
}

void rotate180(void) {
    int angle = 0;
    get_angle();
    currentAngle = convertAngle(currentAngle - 180);
    rotateccw();
    while (angle < 171) {
        __delay_ms(10);
        angle += get_angle();
    }
    stop();
}

void drive_angle(int requiredAngle, unsigned char direction) {
    int angl = 0; // if direction 1, rotate cw, if direction is 0, rotate CCW
    int rA = requiredAngle;
    get_angle();
    //    switch (direction) {
    //        case 0:
    //            currentAngle = convertAngle(currentAngle + requiredAngle);
    //            rotateccw();
    //            break;
    //        case 1:
    //            currentAngle = convertAngle(currentAngle - requiredAngle);
    //            rotatecw(); // rotate counter clockwise
    //            break;
    //    }
    //    while(rA > angl){
    //        __delay_ms(10);
    //        angl += abs(get_angle());
    //    }
    //    stop();
    if (direction == 1) {
        currentAngle = convertAngle(currentAngle + requiredAngle);
        rotatecw(); // rotate clockwise
        while (requiredAngle > angl) {
            __delay_ms(7);
            angl += (-get_angle());
        }
        stop();
    } else if (direction == 0) {
        currentAngle = convertAngle(currentAngle - requiredAngle);
        rotateccw(); // rotate counter clockwise
        while (requiredAngle > angl) {
            __delay_ms(7);
            angl += get_angle();
        }
        stop();
    }
    play_song(0);
}

void stop(void) { // stop the robot
    // stop irobot 
    ser_putch(145);
    ser_putch(0);
    ser_putch(0);
    ser_putch(0);
    ser_putch(0);
}

void travelStraight(void) {// travel 1 meter block by block
    int travelDist = 0;
    int PIDval = 0, lw, rw;
    setCoordinate(currentAngle); // increase location bit
    while (travelDist < 970) { // change number to change amount of distance traveled
        infoDisplay();
        bump();
        if (getVirtualWall()) { // if IR wall is detected
            stop();
            play_song(2);
            travelDist -= 30;
            drive_distance(-travelDist); // return to previous position
            setCoordinate(currentAngle - 180); //return to previous coordinate
            setWall(currentAngle); // set wall in the front;
            return;
        } else if (bumpStop) { // if bump is triggered
            travelDist -= 120;
            drive_distance(-travelDist); // return to previous position
            setCoordinate(currentAngle - 180);
            bumpStop = 0;
            return;
        } else if (p == 0 && distance < 670) { // left and right wall follow, manipulate (distance < number) to change maximum sensing limit
            PIDval = PID();
            lw = baseSpeed - PIDval;
            rw = baseSpeed + PIDval;
            drive(lw, rw);
            travelDist += get_distance();
        } else if (p == 1 && distance < 670) { // manipulate (distance < number) to change maximum sensing limit
            PIDval = PID();
            lw = baseSpeed + PIDval;
            rw = baseSpeed - PIDval;
            drive(lw, rw);
            travelDist += get_distance();
        } else {
            drive(baseSpeed, baseSpeed);
            travelDist += get_distance();
        }
    }
    setMark(currentAngle + 180); // Set mark behind robot
    stop();
//    fixCounter++;
}

void rightWallFollow(void) {
    char steps = 0;
    p = 0;
    spi_transfer(0b00001111);
    __delay_ms(10);
    spi_transfer(0b00001111);
    while (steps < 25) {
        steps++;
        SM_STEP();
        __delay_ms(2);
    }
}