#include <Arduino.h>
#include "step.h"
#include "limit.h"
#include "steering.h"

uint16_t CenterPosition;
uint16_t FullSweepSteps;
uint16_t Position;
uint16_t DesiredPosition;
bool SteeringCalibrated;
uint8_t DesiredPositionHi;
unsigned long SteeringLastMillis;

#define STEERING_STEP_DELAY 10

bool Calibrate()
{
    uint16_t steps;

    // seek full clockwise
    steps = 0;
    while (!LimitCWDown()) {
        Step(true);
        steps++;
        if (steps > 2000) {
            return false;
        }
        delay(STEERING_STEP_DELAY);
    }

    steps = 0;
    while (!LimitCCWDown()) {
        Step(false);
        steps++;
        if (steps > 2000) {
            return false;
        }
        delay(STEERING_STEP_DELAY);
    }
    FullSweepSteps = steps;
    CenterPosition = steps/2;
    Position = 0;
    DesiredPosition = 0;
    SteeringLastMillis = 0;
    SteeringCalibrated = true;

    return true;
}

void SteerAbsolute(uint16_t newPosition)
{
    if (newPosition > FullSweepSteps) {
        newPosition = FullSweepSteps;
    }
    DesiredPosition = newPosition;
}

void SteerAbsoluteHi(uint8_t reg)
{
    DesiredPositionHi = reg;
}

void SteerAbsoluteLo(uint8_t reg)
{
    DesiredPosition = (DesiredPositionHi<<8) | reg;
}

void UpdateSteering() {
    if ((millis()>=SteeringLastMillis) && ((millis()-SteeringLastMillis)<STEERING_STEP_DELAY)) {
        return;
    }

    if (!SteeringCalibrated) {
        return;
    }

    if (Position > DesiredPosition) {
        if (LimitCCWDown()) {
            // don't move, we're at a stop
        } else {
            Step(false);
            Position--;
            SteeringLastMillis = millis();
        }
    }

    if (Position < DesiredPosition) {
        if (LimitCWDown()) {
            // don't move, we're at a stop
        } else {
            Step(true);
            Position++;
            SteeringLastMillis = millis();
        }
    }
}

void SteeringInit()
{
    Calibrate();
    SteerAbsolute(CenterPosition);
}