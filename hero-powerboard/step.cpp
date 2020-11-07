#include <Arduino.h>
#include "step.h"

#define AIN1 (1<<PC2)
#define AIN2 (1<<PC3)
#define BIN1 (1<<PC4)
#define BIN2 (1<<PC5)
#define STEER1_PWM (1<<PB3)
#define STEER2_PWM (1<<PB4)

#define STEP_MASK (AIN1 | AIN2 | BIN1 | BIN2)

// This is what it would look like if the driver's inputs were active high
uint8_t InvStepTable[4] = {AIN1, BIN1, AIN2, BIN2};

// The drivers inputs are active low
uint8_t StepTable[4] = {AIN2|BIN1|BIN2, AIN1|AIN2|BIN2, AIN1|BIN1|BIN2, AIN1|AIN2|BIN1};
uint8_t StepIndex = 0;

void StepInit() {
    DDRC |= STEP_MASK;

    // 100%
    DDRB |= (STEER1_PWM | STEER2_PWM);
    PORTB |= (STEER1_PWM | STEER2_PWM);
}

void SetStepOutput(uint8_t index) {
    // dead time between steps
    PORTC |= STEP_MASK;
    // set the step
    PORTC = (PORTC & (~STEP_MASK)) | StepTable[index];
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
