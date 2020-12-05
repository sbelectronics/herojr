#include <Arduino.h>
#include <Wire.h>
#include "step.h"
#include "headstep.h"
#include "limit.h"
#include "steering.h"
#include "headsteering.h"
#include "drive.h"
#include "slave.h"
#include "encoder.h"
#include "globals.h"

unsigned long tLoopTop;

void setup()
{
    LimitInit();
    StepInit();
    SteeringInit();
    HeadStepInit();
    HeadSteeringInit();
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
#define HEAD_STEP_DELAY 2000
unsigned long tHeadMoved;
void HeadUpdate()
{
    if ((tLoopTop>=tHeadMoved) && ((tLoopTop-tHeadMoved)<HEAD_STEP_DELAY)) {
        return;
    }
    tHeadMoved = tLoopTop;
    HeadStep(true);
}*/

void loop()
{
    tLoopTop = micros();

    SteeringUpdate();
    HeadSteeringUpdate();
    DriveUpdate();
    EncoderUpdate();
}