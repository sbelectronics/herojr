#include <Arduino.h>
#include <Wire.h>
#include "step.h"
#include "limit.h"
#include "steering.h"
#include "drive.h"
#include "slave.h"

void setup()
{
    LimitInit();
    StepInit();
    SteeringInit();
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

void loop()
{
    SteeringUpdate();
    DriveUpdate();
}