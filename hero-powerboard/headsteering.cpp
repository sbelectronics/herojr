#include <Arduino.h>
#include "headstep.h"
#include "headsteering.h"
#include "globals.h"

uint16_t HeadCenterPosition;
uint16_t HeadFullSweepSteps;
uint16_t HeadPosition;
uint16_t HeadDesiredPosition;
uint8_t HeadCalibrationState;
uint16_t HeadCalibrationSteps;
uint8_t HeadDesiredPositionHi;
uint8_t HeadAutoRotate;
bool initialHeadCalibrationDone;
unsigned long tHeadSteeringUpdate;

#define HEAD_HALL (1<<PA4)

#define HEAD_STEERING_STEP_DELAY 2000
#define MAX_HEAD_CALI_STEPS 8000

#define HEAD_CALI_START               0
#define HEAD_CALI_COMPLETE         0x01
#define HEAD_CALI_HALL             0x02
#define HEAD_CALI_CW               0x03
#define HEAD_CALI_HALL_CCW         0x04
#define HEAD_CALI_CCW              0x05
#define HEAD_CALI_NOTSTARTED       0x80
#define HEAD_CALI_FAILURE_HALL_CW  0x81
#define HEAD_CALI_FAILURE_CW       0x82
#define HEAD_CALI_FAILURE_HALL_CCW 0x83
#define HEAD_CALI_FAILURE_CCW      0x84

#define HEAD_ADJ 40


void HeadCalibrationUpdate()
{
    if ((tLoopTop>=tHeadSteeringUpdate) && ((tLoopTop-tHeadSteeringUpdate)<HEAD_STEERING_STEP_DELAY)) {
        return;
    }
    tHeadSteeringUpdate = tLoopTop;

    switch (HeadCalibrationState) {
        case HEAD_CALI_START:
            HeadCalibrationSteps = 0;
            HeadCalibrationState = HEAD_CALI_HALL;
            break;

        // seek clockwise until we find the hall sensor
        case HEAD_CALI_HALL:
            if (HeadHallDown()) {
                HeadCalibrationState = HEAD_CALI_CW;
            } else {
                if (HeadCalibrationSteps> MAX_HEAD_CALI_STEPS) {
                    HeadCalibrationState = HEAD_CALI_FAILURE_HALL_CW;
                }
                HeadStep(true);
                HeadCalibrationSteps++;
            }
            break;

        // seek clockwise until we've left the hall sensor
        case HEAD_CALI_CW:
            if (!HeadHallDown()) {
                HeadCalibrationState = HEAD_CALI_HALL_CCW;
                HeadCalibrationSteps = 0;
            } else {
                if (HeadCalibrationSteps> MAX_HEAD_CALI_STEPS) {
                    HeadCalibrationState = HEAD_CALI_FAILURE_CW;
                }
                HeadStep(true);
                HeadCalibrationSteps++;
            }
            break;

        // seek counterclockwise until we've found the hall sensor again
        case HEAD_CALI_HALL_CCW:
            if (HeadHallDown()) {
                HeadCalibrationState = HEAD_CALI_CCW;
            } else {
                if (HeadCalibrationSteps> MAX_HEAD_CALI_STEPS) {
                    HeadCalibrationState = HEAD_CALI_FAILURE_HALL_CCW;
                }
                HeadStep(false);
                HeadCalibrationSteps++;
            }
            break;

        // seek counterclockwise until we've left the hall sensor
        case HEAD_CALI_CCW:
            if ((!HeadHallDown()) && (HeadCalibrationSteps>30)) {
                HeadCalibrationState = HEAD_CALI_COMPLETE;
                HeadPosition = HeadCenterPosition-(HeadCalibrationSteps/2)+HEAD_ADJ; // 2685 is home
                if (!initialHeadCalibrationDone) {
                    HeadDesiredPosition = HeadCenterPosition;
                    initialHeadCalibrationDone = true;
                }                
            } else {
                if (HeadCalibrationSteps> MAX_HEAD_CALI_STEPS) {
                    HeadCalibrationState = HEAD_CALI_FAILURE_CCW;
                }
                HeadStep(false);
                HeadCalibrationSteps++;
            }

        case HEAD_CALI_FAILURE_CW:
        case HEAD_CALI_FAILURE_HALL_CW:        
        case HEAD_CALI_FAILURE_CCW:
        case HEAD_CALI_FAILURE_HALL_CCW:
            // do nothing
            break;
    }
}

void HeadStartCalibration()
{
    HeadCalibrationState = HEAD_CALI_START;
}

bool HeadCalibrationComplete()
{
    return (HeadCalibrationState == HEAD_CALI_COMPLETE);
}

void HeadSteerAbsolute(uint16_t newPosition)
{
    if (newPosition > HeadFullSweepSteps) {
        newPosition = HeadFullSweepSteps;
    }
    HeadDesiredPosition = newPosition;
}

void HeadSteerAbsoluteHi(uint8_t reg)
{
    HeadDesiredPositionHi = reg;
}

void HeadSteerAbsoluteLo(uint8_t reg)
{
    HeadDesiredPosition = (HeadDesiredPositionHi<<8) | reg;
}

// add something to a head position, wrapping as necessary
uint16_t headPosAdd(int cur, int amount)
{
    cur += amount
    if (cur<0) {
        cur = HeadFullSweepSteps-1;
    }
    if (cur>=HeadFullSweepSteps) {
        cur = 0
    }

    return (uint16_t) cur
}

// return -1 if CCW gets us to the goal faster, or 1 if CW does
int headShortestDirection(uint16_t cur, uint16_t goal)
{
    if (cur<goal) {
        deltaCW = goal-cur;
        deltaCCW = cur+HeadFullSweepSteps - goal;
    } else {
        deltaCW = goal+HeadFullSweepSteps - cur;
        deltaCCW = cur-goal;
    }

    if (deltaCCW < deltaCW) {
        return -1;
    } else {
        return 1;
    }
}

void HeadSteeringUpdate() {
    if (!HeadCalibrationComplete()) {
        HeadCalibrationUpdate();
        return;
    }

    if ((tLoopTop>=tHeadSteeringUpdate) && ((tLoopTop-tHeadSteeringUpdate)<HEAD_STEERING_STEP_DELAY)) {
        return;
    }

    if (HeadAutoRotate == AUTOROTATE_CW) {
        HeadDesiredPosition = headPosAdd(HeadPosition, 1)
    } else if (HeadAutoRotate == AUTOROTATE_CCW) {
        HeadDesiredPosition = headPosAdd(HeadPosition, -1)
    }

    if (HeadPosition == HeadDesiredPosition) {
        return;
    }

    dir = headShortestDirection(HeadPosition, HeadDesiredPosition)

    if (dir > 0) {
        HeadStep(false);
        HeadPosition = headPosAdd(HeadPosition, -1);
        tHeadSteeringUpdate = tLoopTop;
    }

    if (dir < 0) {
        HeadStep(true);
        HeadPosition = headPosAdd(HeadPosition, 1);
        tHeadSteeringUpdate = tLoopTop;
    }
}

bool HeadHallDown() {
    return ((PINA & HEAD_HALL) == 0);
}


void HeadSteeringInit()
{
     // set as inputs and turn on pullups
    DDRA &= (~HEAD_HALL);
    PORTA |= HEAD_HALL;

    initialHeadCalibrationDone = false;
    HeadAutoRotate = AUTOROTATE_NONE;
    HeadFullSweepSteps = 5370;
    HeadCenterPosition = 2685;
    HeadPosition = 0;
    HeadDesiredPosition = 0;
    HeadCalibrationState = HEAD_CALI_NOTSTARTED;

    HeadStartCalibration();
    HeadSteerAbsolute(HeadCenterPosition);
}