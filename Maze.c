
#include "Maze.h"

void chooseRouteT(void) { // Tremaux path finding algorithm, front path highest priority, left second, right third
    if (getWall(currentAngle) == 0 && getMark(currentAngle) == 0) {
        return;
    } else if (getWall(currentAngle - 90) == 0 && getMark(currentAngle - 90) == 0) {
        rotate90ccw();
    } else if (getWall(currentAngle + 90) == 0 && getMark(currentAngle + 90) == 0) {
        rotate90cw();
    } else if (getWall(currentAngle + 180) == 0 && getMark(currentAngle + 180) == 0) {
        rotate180();
    } else if (getWall(currentAngle + 180) == 0 && getMark(currentAngle + 180) == 1) {
        rotate180();
    } else if (getWall(currentAngle) == 0 && getMark(currentAngle) == 1) {
        return;
    } else if (getWall(currentAngle - 90) == 0 && getMark(currentAngle - 90) == 1) {
        rotate90ccw();
    } else if (getWall(currentAngle + 90) == 0 && getMark(currentAngle + 90) == 1) {
        //        drive_angle(90, 1);
        rotate90cw();
    } else {
        while (1) {
            play_song(5);
            __delay_ms(5000);
            drive_angle(360, 1);
        }
    }
}

void backTrack(void) { // back track algorithm back to starting point, follows 1 mark path
    if (getWall(currentAngle) == 0 && getMark(currentAngle) == 1) {
        return;
    } else if (getWall(currentAngle - 90) == 0 && getMark(currentAngle - 90) == 1) {
        //        drive_angle(90, 0);
        rotate90ccw();
    } else if (getWall(currentAngle + 90) == 0 && getMark(currentAngle + 90) == 1) {
        //        drive_angle(90, 1);
        rotate90cw();
    } else {
        rotate180();
        __delay_ms(1000);
    }
    //        drive_angle(180, 1);
}

void setCoordinate(int angle) { // Increase or decrease current coordinate base on the input angle
    angle = convertAngle(angle);
    switch (angle) {
        case 0: ++y;
            break;
        case 90: ++x;
            break;
        case 180: --y;
            break;
        case 270: --x;
            break;
    }
}

int convertAngle(int angle) { // convert angle value larger than 360 to 0 and less than 0 to 270
    if (angle < 0)
        angle += 360;
    else if (angle >= 360)
        angle -= 360;
    return angle;
}

void mazeInitialise(void) {// initialize all map data to 0
    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= 3; j++) {
            coordinate[i][j] = 0b00000000;
            mark[i][j] = 0b00000000;
        }
    }
}

void setWall(int angle) { // set wall bit base on input angle
    angle = convertAngle(angle);
    switch (angle) {
        case 0: coordinate[x][y] |= 0b00001000; // set north wall bit
            break;
        case 90: coordinate[x][y] |= 0b00000100; // set east wall bit
            break;
        case 180: coordinate[x][y] |= 0b00000010; // set south wall bit
            break;
        case 270: coordinate[x][y] |= 0b00000001; // set west wall bit
            break;
    }
}

char getWall(int angle) {
    char val;
    angle = convertAngle(angle);
    switch (angle) {
        case 0: val = (coordinate[x][y] & 0b1000) >> 3; // get north wall bit
            break;
        case 90: val = (coordinate[x][y] & 0b0100) >> 2; // get east wall bit
            break;
        case 180: val = (coordinate[x][y] & 0b0010) >> 1; // get south wall bit
            break;
        case 270: val = coordinate[x][y] & 0b0001; // get west wall bit
            break;
    }
    return val;
}

/***************************************Quickest path back******************************/

char getWall1(int angle) {
    char val;
    switch (angle) {
        case 90: val = (coordinate[1][3] & 0b0010) >> 2;
        case 270: val = (coordinate[1][3] & 0b0001);
    }
    return val;
}

char getWall2(int angle) {
    char val;
    switch (angle) {
        case 0: val = (coordinate[2][2] & 0b1000) >> 3;
        case 180: val = (coordinate[2][2] & 0b0010) >> 1;
    }
    return val;
}

char getWall3(int angle) {
    char val;
    switch (angle) {
        case 0: val = (coordinate[0][2] & 0b1000) >> 3;
        case 180: val = (coordinate[0][2] & 0b0010) >> 1;
    }
    return val;
}

void angleDirection(int angle) {
    if (angle == 0)
        return;
    else if (angle >= 0) {
        drive_angle(angle, 1);
    }
    else if (angle < 0) {
        angle = -angle;
        drive_angle(angle, 0);
    }
}

void travelBack(void) {
    //    while (!terminate) {
    int angle = 0;
    if (x == 2 && y == 3) {
        if (getWall1(90) == 0 && getWall(270) == 0) {
            rightWallFollow();
            angle = 270 - currentAngle;
            angleDirection(angle);
            travelStraight();
            rotate90ccw();
            travelStraight();
            stop();
            terminate = 1;
            play_song(1);
            return;
        } else {
            rightWallFollow();
            angle = 180 - currentAngle;
            angleDirection(angle);
            travelStraight();
            travelStraight();
            rotate90cw();
            travelStraight();
            travelStraight();
            rotate90cw();
            travelStraight();
            travelStraight();
            rotate90cw();
            travelStraight();
            rotate90cw();
            travelStraight();
            stop();
            terminate = 1;
            play_song(1);
            return;
        }
    } else if (x == 0 && y == 1) {
        if (getWall3(180) == 0 && getWall3(0) == 0 && getWall1(270) == 0 && getWall(0) == 0) {
            rightWallFollow();
            angle = 0 - currentAngle;
            angleDirection(angle);
            travelStraight();
            travelStraight();
            rotate90cw();
            travelStraight();
            rotate90cw();
            travelStraight();
            stop();
            terminate = 1;
            play_song(1);
            return;
        } else {
            rightWallFollow();
            angle = 90 - currentAngle;
            angleDirection(angle);
            travelStraight();
            travelStraight();
            rotate90ccw();
            travelStraight();
            travelStraight();
            rotate90ccw();
            travelStraight();
            rotate90ccw();
            travelStraight();
            stop();
            terminate = 1;
            play_song(1);
            return;
        }
    } else if (x == 2 && y == 1) {
        if (getWall2(180) == 0 && getWall2(0) == 0 && getWall1(90) == 0 && getWall(0) == 0) {
            rightWallFollow();
            angle = 0 - currentAngle;
            angleDirection(angle);
            travelStraight();
            travelStraight();
            rotate90ccw();
            travelStraight();
            rotate90ccw();
            travelStraight();
            stop();
            terminate = 1;
            play_song(1);
            return;
        } else {
            rightWallFollow();
            angle = 270 - currentAngle;
            angleDirection(angle);
            travelStraight();
            travelStraight();
            rotate90cw();
            travelStraight();
            travelStraight();
            rotate90cw();
            travelStraight();
            rotate90cw();
            travelStraight();
            stop();
            terminate = 1;
            play_song(1);
            return;
        }
        //        } else if (getWall(currentAngle) == 0 && getMark(currentAngle) == 1) {
        //            travelStraight();
        //            scan();
        //        } else if (getWall(currentAngle - 90) == 0 && getMark(currentAngle - 90) == 1) {
        //            //        drive_angle(90, 0);
        //            rotate90ccw();
        //            travelStraight();
        //            scan();
        //        } else if (getWall(currentAngle + 90) == 0 && getMark(currentAngle + 90) == 1) {
        //            //        drive_angle(90, 1);
        //            rotate90cw();
        //            travelStraight();
        //            scan();
        //        } else {
        //            rotate180();
        //            travelStraight();
        //            scan();
        //        }
    } else
        backTrack();
}

/******************************end of quick path*****************************/

void setMark(int angle) { // set mark the robot has been base on angle input
    char tempMark = 0;
    angle = convertAngle(angle);
    switch (angle) {
        case 0: // traveled north
            tempMark = (mark[x][y] & 0b11000000) >> 6;
            if (tempMark == 0)
                mark[x][y] |= 0b01000000;
            if (tempMark == 1)
                mark[x][y] |= 0b10000000;
            break;
        case 90: // traveled east
            tempMark = (mark[x][y] & 0b00110000) >> 4;
            if (tempMark == 0)
                mark[x][y] |= 0b00010000;
            if (tempMark == 1)
                mark[x][y] |= 0b00100000;
            break;
        case 180: // traveled south
            tempMark = (mark[x][y] & 0b00001100) >> 2;
            if (tempMark == 0)
                mark[x][y] |= 0b00000100;
            if (tempMark == 1)
                mark[x][y] |= 0b00001000;
            break;
        case 270: // traveled west
            tempMark = (mark[x][y] & 0b00000011);
            if (tempMark == 0)
                mark[x][y] |= 0b00000001;
            if (tempMark == 1)
                mark[x][y] |= 0b00000010;
            break;
    }
}

char getMark(int angle) {
    char tempMark = 0;
    angle = convertAngle(angle);
    switch (angle) {
        case 0: tempMark = ((mark[x][y] & 0b11000000) >> 6);
            break;
        case 90: tempMark = ((mark[x][y] & 0b00110000) >> 4);
            break;
        case 180: tempMark = ((mark[x][y] & 0b00001100) >> 2);
            break;
        case 270: tempMark = (mark[x][y] & 0b00000011);
            break;
    }
    return tempMark;
}
