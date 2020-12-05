#include <Arduino.h>
#include "globals.h"
#include "keyboard.h"
#include "leds.h"

#define KBC0 (1<<PC2)
#define KBC1 (1<<PC3)
#define KBC2 (1<<PC4)
#define KBC3 (1<<PC5)

#define KBR0 (1<<PA4)
#define KBR1 (1<<PA5)
#define KBR2 (1<<PA6)
#define KBR3 (1<<PA7)

#define CMASK (KBC0 | KBC1 | KBC2 | KBC3)
#define AMASK (KBR0 | KBR1 | KBR2 | KBR3)

#define KEY_DEBOUNCE 500

unsigned long tKeypadUpdate;
uint16_t keyState;

/* Important note
 *
 * Unset the JTAG bit or you'll be sorry. Port C won't work properly.
 */

void KeyboardInit() {
    DDRA &= (~AMASK);
    PORTA |= AMASK; // enable pullups

    DDRC |= CMASK;

    tKeypadUpdate = 0;
}

uint16_t KeyboardPoll() {
    uint16_t newKeyState = 0;

    PORTC = (PORTC | CMASK) & (~KBC3);
    delayMicroseconds(1);
    if ((PINA & KBR0) == 0) {
        newKeyState |= KC;
    }
    if ((PINA & KBR1) == 0) {
        newKeyState |= K8;
    }
    if ((PINA & KBR2) == 0) {
        newKeyState |= K4;
    }
    if ((PINA & KBR3) == 0) {
        newKeyState |= K0;
    }

    PORTC = (PORTC | CMASK) & (~KBC2);
    delayMicroseconds(1);
    if ((PINA & KBR0) == 0) {
        newKeyState |= KD;
    }
    if ((PINA & KBR1) == 0) {
        newKeyState |= K9;
    }
    if ((PINA & KBR2) == 0) {
        newKeyState |= K5;
    }
    if ((PINA & KBR3) == 0) {
        newKeyState |= K1;
    }

    PORTC = (PORTC | CMASK) & (~KBC1);
    delayMicroseconds(1);
    if ((PINA & KBR0) == 0) {
        newKeyState |= KE;
    }
    if ((PINA & KBR1) == 0) {
        newKeyState |= KA;
    }
    if ((PINA & KBR2) == 0) {
        newKeyState |= K6;
    }
    if ((PINA & KBR3) == 0) {
        newKeyState |= K2;
    }

    PORTC = (PORTC | CMASK) & (~KBC0);
    delayMicroseconds(1);
    if ((PINA & KBR0) == 0) {
        newKeyState |= KF;
    }
    if ((PINA & KBR1) == 0) {
        newKeyState |= KB;
    }
    if ((PINA & KBR2) == 0) {
        newKeyState |= K7;
    }
    if ((PINA & KBR3) == 0) {
        newKeyState |= K3;
    }

    PORTC = (PORTC | CMASK);

    return newKeyState;
}

void KeyboardUpdate()
{
    uint8_t i;

    keyState = KeyboardPoll();

    for (i=0; i<8; i++) {
        uint8_t mask;
        mask = (1<<i);
        if (((keyState & mask)!=0) || (((keyState>>8) & mask)!=0)) {
            SetLedBright(i, 255);
        } else {
            SetLedBright(i, 0);
        }
    }
}