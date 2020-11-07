#include <Arduino.h>
#include <Wire.h>
#include "step.h"
#include "headstep.h"
#include "limit.h"
#include "steering.h"
#include "drive.h"
#include "slave.h"
#include "encoder.h"

void setup()
{
    LimitInit();
    StepInit();
    SteeringInit();
    HeadStepInit();
    EncoderInit();
    SlaveInit();
}

// Use the limit switch inputs to test the stepper motor
void MotorTest()
{
    if (LimitCWDown()) {
        Step(true);
    } else if (LimitCCWDown()) {
        Step(false);
    }
    delay(250);
}

/*
#define HEAD_STEP_DELAY 1000
unsigned long HeadLastMicros;
void HeadUpdate()
{
    if ((micros()>=HeadLastMicros) && ((micros()-HeadLastMicros)<HEAD_STEP_DELAY)) {
        return;
    }
    HeadLastMicros = micros();
    HeadStep(true);
}
*/

#define HEAD_STEP_DELAY 1
unsigned long HeadLastMillis;
void HeadUpdate()
{
    if ((millis()>=HeadLastMillis) && ((millis()-HeadLastMillis)<HEAD_STEP_DELAY)) {
        return;
    }
    HeadLastMillis = millis();
    HeadStep(true);
}

void loop()
{
    SteeringUpdate();
    HeadUpdate();
    DriveUpdate();
    EncoderUpdate();
}