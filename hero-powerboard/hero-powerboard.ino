#include <Arduino.h>
#include <Wire.h>
#include "step.h"
#include "limit.h"
#include "steering.h"

void setup()
{
    LimitInit();
    StepInit();
    SteeringInit();
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
    UpdateSteering();
}