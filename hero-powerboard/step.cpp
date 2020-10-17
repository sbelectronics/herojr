#include <Arduino.h>
#include "step.h"

// It would have made a whole lot of sense to put these all on the same port!!! 
#define AIN1 (1<<PC3)
#define AIN2 (1<<PB0)
#define BIN1 (1<<PB6)
#define BIN2 (1<<PB7)
#define STEER_PWM (1<<PB1)

#define CMASK AIN1
#define BMASK (AIN2 | BIN1 | BIN2)

// This is what it would look like if the driver's inputs were active high
uint8_t InvStepTable[4] = {AIN1, BIN1, AIN2, BIN2};

// The drivers inputs are active low
uint8_t StepTableB[4] = {AIN2|BIN1|BIN2, AIN2|BIN2, BIN1|BIN2, AIN2|BIN1};
uint8_t StepTableC[4] = {0, AIN1, AIN1, AIN1};
uint8_t StepIndex = 0;

void StepInit() {
    DDRB |= BMASK;
    DDRC |= CMASK;

    // 100%
    DDRB |= STEER_PWM;
    PORTB |= STEER_PWM;
}

void SetStepOutput(uint8_t index) {
    // dead time between steps
    PORTB |= BMASK;
    PORTC |= CMASK;
    // set the step
    PORTB = (PORTB & (~BMASK)) | StepTableB[index];
    PORTC = (PORTC & (~CMASK)) | StepTableC[index];
}

void Step(bool clockwise) {
    if (!clockwise) {
        StepIndex += 1;
        if (StepIndex > 3) {
            StepIndex = 0;
        }
    } else {
        if (StepIndex == 0) {
            StepIndex = 3;
        } else {
            StepIndex -= 1;
        }
    }
    SetStepOutput(StepIndex);
}
