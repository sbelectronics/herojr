#include <Arduino.h>
#include "encoder.h"

// It would have made a whole lot of sense to put these all on the same port!!! 
#define WHEEL_ENCODER (1<<PA3)

#define ENCODER_DELAY = 1

uint16_t WheelEncoderCount;
uint8_t WheelEncoderLast;
unsigned long WheelEncoderLastMillis;

void EncoderInit() {
    DDRA &= (~WHEEL_ENCODER);
    WheelEncoderLast = WHEEL_ENCODER;
    WheelEncoderCount = 0;
    WheelEncoderLastMillis = 0;
}

void EncoderUpdate() {
    uint8_t we = (PINA & WHEEL_ENCODER);
    if (we!=WheelEncoderLast) {
        // debounce
        if (millis() == WheelEncoderLastMillis) {
            return;
        }
        WheelEncoderLastMillis = millis();
        WheelEncoderLast = we;

        if (we == 0) {
            WheelEncoderCount ++;
        }
    }
}