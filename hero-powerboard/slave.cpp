#include <Arduino.h>
#include <Wire.h>
#include "step.h"
#include "limit.h"
#include "steering.h"
#include "slave.h"
#include "regs.h"

#define RX_BUFFER_SIZE 16

uint8_t RegPosition;

void SlaveReceive(int howMany) {
    if (howMany < 1)
    {
        // Sanity-check
        return;
    }
    if (howMany > 16)
    {
        // Also insane number
        return;
    }

    RegPosition = Wire.read();
    howMany --;

    while (howMany>0) {
        uint8_t data = Wire.read();
        switch (RegPosition) {
            case REG_DESIREDPOSITION_HI:
                // NOTE: Will not be read-back until the low byte is also written
                SteerAbsoluteHi(data);
                break;
            case REG_DESIREDPOSITION_LO:
                SteerAbsoluteLo(data);
                break;
        }
        RegPosition++;
        howMany--;
    }
}

void SlaveRequest() {
    uint8_t data = 0;

    // NOTE: Must be kept fast to avoid clock stretching.

    switch (RegPosition) {
        case REG_DESIREDPOSITION_HI:
            data = DesiredPosition>>8;
            break;
        case REG_DESIREDPOSITION_LO:
            data = DesiredPosition&0xFF;
            break;
        case REG_POSITION_HI:
            data = Position>>8;
            break;
        case REG_POSITION_LO:
            data = Position&0xFF;
            break;
        case REG_FULLSWEEPSTEPS_HI:
            data = FullSweepSteps>>8;
            break;
        case REG_FULLSWEEPSTEPS_LO:
            data = FullSweepSteps&0xFF;
            break;
        case REG_CENTERPOSITION_HI:
            data = CenterPosition>>8;
            break;
        case REG_CENTERPOSITION_LO:
            data = CenterPosition&0xFF;
            break;            
        case REG_LIMITS:
            data = (LimitCWDown() ? 1 : 0) | (LimitCCWDown() ? 2 : 0);
            break;
    }
    Wire.write(data);

    RegPosition++;
}

void SlaveInit() {
    RegPosition = 0;
    Wire.begin(4);
    Wire.onReceive(SlaveReceive);
    Wire.onRequest(SlaveRequest);
}