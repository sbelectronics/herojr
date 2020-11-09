#include <Arduino.h>
#include "step.h"
#include "limit.h"
#include "steering.h"
#include "globals.h"

uint16_t CenterPosition;
uint16_t FullSweepSteps;
uint16_t Position;
uint16_t DesiredPosition;
uint8_t CalibrationState;
uint16_t CalibrationSteps;
uint8_t DesiredPositionHi;
bool initialCalibrationDone;
unsigned long tSteeringUpdate;

#define STEERING_STEP_DELAY 10000

#define CALI_START          0
#define CALI_COMPLETE    0x01
#define CALI_CW          0x02
#define CALI_CCW         0x03
#define CALI_NOTSTARTED  0x80
#define CALI_FAILURE_CW  0x81
#define CALI_FAILURE_CCW 0x82

void CalibrationUpdate()
{
    if ((tLoopTop>=tSteeringUpdate) && ((tLoopTop-tSteeringUpdate)<STEERING_STEP_DELAY)) {
        return;
    }
    tSteeringUpdate = tLoopTop;

    switch (CalibrationState) {
        case CALI_START:
            CalibrationSteps = 0;
            CalibrationState = CALI_CW;
            break;

        case CALI_CW:
            if (LimitCWDown()) {
                CalibrationState = CALI_CCW;
                CalibrationSteps = 0;
            } else {
                if (CalibrationSteps>1500) {
                    CalibrationState = CALI_FAILURE_CW;
                }
                Step(true);
                CalibrationSteps++;
            }
            break;

        case CALI_CCW:
            if (LimitCCWDown()) {
                FullSweepSteps = CalibrationSteps;
                CenterPosition = CalibrationSteps/2;
                Position = 0;
                if (!initialCalibrationDone) {
                    DesiredPosition = CenterPosition;
                    initialCalibrationDone = true;
                }
                CalibrationState = CALI_COMPLETE;
            } else {
                if (CalibrationSteps>1500) {
                    CalibrationState = CALI_FAILURE_CCW;
                }
                Step(false);
                CalibrationSteps++;
            }
            break;

        case CALI_FAILURE_CW:
        case CALI_FAILURE_CCW:
            // do nothing
            break;
    }
}

void StartCalibration()
{
    CalibrationState = CALI_START;
}

bool CalibrationComplete()
{
    return (CalibrationState == CALI_COMPLETE);
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

void SteeringUpdate() {
    if (!CalibrationComplete()) {
        CalibrationUpdate();
        return;
    }

    if ((tLoopTop>=tSteeringUpdate) && ((tLoopTop-tSteeringUpdate)<STEERING_STEP_DELAY)) {
        return;
    }

    if (Position > DesiredPosition) {
        if (LimitCCWDown()) {
            // don't move, we're at a stop
        } else {
            Step(false);
            Position--;
            tSteeringUpdate = tLoopTop;
        }
    }

    if (Position < DesiredPosition) {
        if (LimitCWDown()) {
            // don't move, we're at a stop
        } else {
            Step(true);
            Position++;
            tSteeringUpdate = tLoopTop;
        }
    }
}

void SteeringInit()
{
    initialCalibrationDone = false;
    FullSweepSteps = 0;
    CenterPosition = 0;
    Position = 0;
    DesiredPosition = 0;
    CalibrationState = CALI_NOTSTARTED;

    StartCalibration();
    SteerAbsolute(CenterPosition);
}