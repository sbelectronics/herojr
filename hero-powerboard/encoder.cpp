#include <Arduino.h>
#include "globals.h"
#include "encoder.h"

// It would have made a whole lot of sense to put these all on the same port!!! 
#define WHEEL_ENCODER (1<<PA3)

#define ENCODER_DELAY = 1000

uint16_t WheelEncoderCount;
uint8_t WheelEncoderLast;
unsigned long tWheelEncoderChange;

void EncoderInit() {
    DDRA &= (~WHEEL_ENCODER);
    WheelEncoderLast = WHEEL_ENCODER;
    WheelEncoderCount = 0;
    tWheelEncoderChange = 0;
}

void EncoderUpdate() {
    uint8_t we = (PINA & WHEEL_ENCODER);
    if (we!=WheelEncoderLast) {
        // debounce
        if (tLoopTop == tWheelEncoderChange) {
            return;
        }
        tWheelEncoderChange = tLoopTop;
        WheelEncoderLast = we;

        if (we == 0) {
            WheelEncoderCount ++;
        }
    }
}