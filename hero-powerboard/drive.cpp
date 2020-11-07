#include <Arduino.h>
#include "step.h"

// It would have made a whole lot of sense to put these all on the same port!!! 
#define CIN1 (1<<PC6)
#define CIN2 (1<<PC7)
// note uses arduino pin numbers 12 = analog out on PD4
#define DRIVE_PWM_PIN 12
#define DRIVE_PWM (1<<PD4)

#define CMASK (CIN1 | CIN2)

#define REVERSE_DELAY 250

unsigned long LastStoppedMillis;
uint8_t Speed;
uint8_t DesiredSpeed;
bool Forward;
bool DesiredForward;

void _SetDriveDirection(uint8_t forward)
{
    if (forward) {
        PORTC = (PORTC & (~CMASK)) | CIN2;
    } else {
        PORTC = (PORTC & (~CMASK)) | CIN1;
    }
    Forward = forward;
}

void _SetDriveSpeed(uint8_t speed)
{
    analogWrite(DRIVE_PWM_PIN, speed);
    Speed = speed;

    if (speed == 0) {
        LastStoppedMillis = millis();
    }
}

void SetDesiredForard(uint8_t forward)
{
    DesiredForward = forward;
}

void SetDesiredSpeed(uint8_t speed)
{
    DesiredSpeed = speed;
}

void DriveInit() {
    LastStoppedMillis = 0;
    Speed = 0;
    Forward = true;

    DDRC |= CMASK;

    DDRD |= DRIVE_PWM;
    analogWrite(DRIVE_PWM_PIN, 0);

    _SetDriveDirection(true);
}

void ForwardReverseUpdate()
{
    if (Speed != 0) {
        _SetDriveSpeed(0);
        return;
    }

    // some deadtime when changing direction
    if ((millis()>=LastStoppedMillis) && ((millis()-LastStoppedMillis)<REVERSE_DELAY)) {
        return;
    }

    _SetDriveDirection(DesiredForward);
}

void DriveUpdate()
{
    // If we're going the wrong direction, fix that.
    if (DesiredForward != Forward) {
        ForwardReverseUpdate();
        return;
    }

    if (DesiredSpeed != Speed) {
        _SetDriveSpeed(DesiredSpeed);
    }
}

