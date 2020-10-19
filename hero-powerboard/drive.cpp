#include <Arduino.h>
#include "step.h"

// It would have made a whole lot of sense to put these all on the same port!!! 
#define CIN1 (1<<PD2)
#define CIN2 (1<<PD3)
#define DRIVE_PWM_PIN PD5
#define DRIVE_PWM (1<<DRIVE_PWM_PIN)

#define DMASK (CIN1 | CIN2)

#define REVERSE_DELAY 250

unsigned long LastStoppedMillis;
uint8_t Speed;
uint8_t DesiredSpeed;
bool Forward;
bool DesiredForward;

void _SetDriveDirection(uint8_t forward)
{
    if (forward) {
        PORTD = (PORTD & (~DMASK)) | CIN2;
    } else {
        PORTD = (PORTD & (~DMASK)) | CIN1;
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

    DDRD |= DMASK;

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

