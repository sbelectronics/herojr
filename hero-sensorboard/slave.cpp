#include <Arduino.h>
#include <Wire.h>
#include <util/crc16.h>
#include "slave.h"
#include "regs.h"

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
            case REG_ECHO:
                EchoReg = data;
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
        case REG_ECHO:
            data = EchoReg;
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
    Wire.begin(6);
    Wire.onReceive(SlaveReceive);
    Wire.onRequest(SlaveRequest);
}