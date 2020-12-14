#include <Arduino.h>
#include <Wire.h>
#include <util/crc16.h>
#include "step.h"
#include "limit.h"
#include "steering.h"
#include "headsteering.h"
#include "slave.h"
#include "regs.h"
#include "drive.h"
#include "limit.h"
#include "encoder.h"

#define ERROR_CRC 0xFF
#define ERROR_SIZE 0xFE
#define ERROR_UNINITIALIZED 0xFD
#define ERROR_SUCCESS 0

#define RX_BUFFER_SIZE 16

uint8_t RegPosition;
uint8_t receive_error;
uint8_t send_crc, next_crc;
uint8_t EchoReg;

void SlaveReceive(int howMany) {
    uint8_t buf[RX_BUFFER_SIZE];
    uint8_t i;
    uint8_t crc;

    // must be 2 bytes because crc is always sent
    if (howMany < 2)
    {
        // Sanity-check
        return;
    }
    if (howMany > RX_BUFFER_SIZE)
    {
        // Also insane number
        return;
    }

    crc = 0;
    for (i=0; i<howMany; i++) {
        buf[i] = Wire.read();
        crc = _crc8_ccitt_update(crc, buf[i]);
    }

    // we included the crc byte in the computation
    // so at this point it should be zero.
    if (crc!=0) {
        // crc error
        receive_error = ERROR_CRC;
        return;
    }

    receive_error = ERROR_SUCCESS;
    send_crc = 0;

    RegPosition = buf[0];

    for (i=1; i<howMany-1; i++) {
        uint8_t data = buf[i];
        switch (RegPosition) {
            case REG_DESIREDPOSITION_HI:
                // NOTE: Will not be read-back until the low byte is also written
                SteerAbsoluteHi(data);
                break;
            case REG_DESIREDPOSITION_LO:
                SteerAbsoluteLo(data);
                break;
            case REG_HEADDESIREDPOSITION_HI:
                // NOTE: Will not be read-back until the low byte is also written
                HeadSteerAbsoluteHi(data);
                break;
            case REG_HEADDESIREDPOSITION_LO:
                HeadSteerAbsoluteLo(data);
                break;
            case REG_DESIREDSPEED:
                DesiredSpeed = data;
                break;
            case REG_DESIREDFOWARD:
                DesiredForward = data;
                break;
            case REG_ECHO:
                EchoReg = data;
                break;
            case REG_HEADAUTOROTATE:
                HeadAutoRotate = data;
                break;                
        }
        RegPosition++;
    }
}

void SlaveRequest() {
    uint8_t data = 0;

    if (send_crc) {
        Wire.write(next_crc);
        send_crc = 0;
        return;
    }

    if (receive_error) {
        // on a receive error, send a two-byte sequence (0xFF,ErrorCode)
        // NOTE: The CRC for a valid 0xFF is 0xF3, so never use 0xF3 as an errorcode
        Wire.write(0xFF);
        next_crc = receive_error;
        send_crc = 1;
        return;
    }    

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
        case REG_CALIBRATIONSTEPS_HI:
            data = CalibrationSteps>>8;
            break;
        case REG_CALIBRATIONSTEPS_LO:
            data = CalibrationSteps&0xFF;
            break;

        case REG_HEADDESIREDPOSITION_HI:
            data = HeadDesiredPosition>>8;
            break;
        case REG_HEADDESIREDPOSITION_LO:
            data = HeadDesiredPosition&0xFF;
            break;
        case REG_HEADPOSITION_HI:
            data = HeadPosition>>8;
            break;
        case REG_HEADPOSITION_LO:
            data = HeadPosition&0xFF;
            break;
        case REG_HEADFULLSWEEPSTEPS_HI:
            data = HeadFullSweepSteps>>8;
            break;
        case REG_HEADFULLSWEEPSTEPS_LO:
            data = HeadFullSweepSteps&0xFF;
            break;
        case REG_HEADCENTERPOSITION_HI:
            data = HeadCenterPosition>>8;
            break;
        case REG_HEADCENTERPOSITION_LO:
            data = HeadCenterPosition&0xFF;
            break;
        case REG_HEADCALIBRATIONSTEPS_HI:
            data = HeadCalibrationSteps>>8;
            break;
        case REG_HEADCALIBRATIONSTEPS_LO:
            data = HeadCalibrationSteps&0xFF;
            break;

        case REG_WHEELENCODER_HI:
            data = WheelEncoderCount>>8;
            break;
        case REG_WHEELENCODER_LO:
            data = WheelEncoderCount&0xFF;
            break;                               
        case REG_LIMITS:
            data = (LimitCWDown() ? 1 : 0) | (LimitCCWDown() ? 2 : 0) | (HeadHallDown() ? 4 : 0);
            break;
        case REG_SPEED:
            data = Speed;
            break;
        case REG_DESIREDSPEED:
            data = DesiredSpeed;
            break;
        case REG_FORWARD:
            data = Forward ? 1 : 0;
            break;
        case REG_DESIREDFOWARD:
            data = DesiredForward ? 1 : 0;
            break;
        case REG_ECHO:
            data = EchoReg;
            break;
        case REG_CALIBRATIONSTATE:
            data = CalibrationState;
            break;
        case REG_HEADCALIBRATIONSTATE:
            data = HeadCalibrationState;
            break;
        case REG_HEADAUTOROTATE:
            data = HeadAutoRotate;
            break;
    }

    Wire.write(data);
    next_crc = _crc8_ccitt_update(0, data);
    send_crc = 1;    

    RegPosition++;
}

void SlaveInit() {
    receive_error = ERROR_UNINITIALIZED;
    RegPosition = 0;
    EchoReg = 0;
    Wire.begin(4);
    Wire.onReceive(SlaveReceive);
    Wire.onRequest(SlaveRequest);
}