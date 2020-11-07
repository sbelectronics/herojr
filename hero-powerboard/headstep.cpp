#include <Arduino.h>
#include "headstep.h"

#define AIN1 (1<<PD0)
#define AIN2 (1<<PD1)
#define BIN1 (1<<PD2)
#define BIN2 (1<<PD3)
#define HEAD1_PWM (1<<PD6)
#define HEAD2_PWM (1<<PD7)

// note uses arduino pin numbers 14 = analog out on PD6
#define HEAD1_PWM_ANA 14
// note uses arduino pin numbers 15 = analog out on PD7
#define HEAD2_PWM_ANA 15

#define STEP_MASK (AIN1 | AIN2 | BIN1 | BIN2)

//#define MICROSTEP

#ifndef MICROSTEP
#define NUM_STEPS 4
#else
#define NUM_STEPS 16
#endif

// A more responsible programmer than myself would have written step.cpp
// in a way that was reusable rather than copy/pasting it here.

// The drivers inputs are active low
uint8_t HeadStepTable[4] = {AIN2|BIN1|BIN2, AIN1|AIN2|BIN2, AIN1|BIN1|BIN2, AIN1|AIN2|BIN1};
uint8_t HeadStepIndex = 0;

uint8_t HeadStepTableInv[16] =
                               {AIN1|BIN2, AIN1|BIN2, AIN1|BIN2, AIN1,  // rise of AIN1, decline of BIN2
                                BIN1|AIN1, BIN1|AIN1, BIN1|AIN1, BIN1,  // rise of BIN1, decline of AIN1
                                AIN2|BIN1, AIN2|BIN1, AIN2|BIN1, AIN2,  // rise of AIN2, decline of BIN1
                                BIN2|AIN2, BIN2|AIN2, BIN2|AIN2, BIN2}; // rise of BIN2, decline of AIN2

uint8_t PWM1[16] =
                   {98, 180, 236, 255,
                    236, 180, 98, 0,
                    98, 180, 236, 255,
                    236, 180, 98, 0};

uint8_t PWM2[16] =
                   {236, 180, 98, 0,
                    98, 180, 236, 255,
                    236, 180, 98, 0,
                    98, 180, 236, 255};

void HeadStepInit() {
    DDRD |= STEP_MASK;

    #ifndef MICROSTEP
    // 100%
    DDRD |= (HEAD1_PWM | HEAD2_PWM);
    analogWrite(HEAD1_PWM_ANA, 255);
    analogWrite(HEAD2_PWM_ANA, 255);

    #else
    // 0%
    DDRD |= (HEAD1_PWM | HEAD2_PWM);
    analogWrite(HEAD1_PWM_ANA, 0);
    analogWrite(HEAD2_PWM_ANA, 0);
    #endif
}

void HeadSetStepOutputMicro(uint8_t index) {
    // dead time between steps
    PORTD |= STEP_MASK;
    // set the step
    analogWrite(HEAD1_PWM_ANA, PWM1[index]);
    analogWrite(HEAD2_PWM_ANA, PWM2[index]);
    PORTD = (PORTD & (~STEP_MASK)) | (STEP_MASK & (~HeadStepTableInv[index]));
}

void HeadSetStepOutput(uint8_t index) {
    // dead time between steps
    PORTD |= STEP_MASK;
    // set the step
    PORTD = (PORTD & (~STEP_MASK)) | HeadStepTable[index];
}

void HeadStep(bool clockwise) {
    if (!clockwise) {
        HeadStepIndex += 1;
        if (HeadStepIndex >= NUM_STEPS) {
            HeadStepIndex = 0;
        }
    } else {
        if (HeadStepIndex == 0) {
            HeadStepIndex = NUM_STEPS-1;
        } else {
            HeadStepIndex -= 1;
        }
    }
#ifndef MICROSTEP
    HeadSetStepOutput(HeadStepIndex);
#else
    HeadSetStepOutputMicro(HeadStepIndex);
#endif
}
