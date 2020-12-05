#include <Arduino.h>
#include "globals.h"
#include "leds.h"

#define LED0 (1<<PD4)
#define LED1 (1<<PD5)
#define LED2 (1<<PD6)
#define LED3 (1<<PD7)
#define LED4 (1<<PB3)
#define LED5 (1<<PB4)
#define LED6 (1<<PB0)
#define LED7 (1<<PB1)
#define LEDPWR (1<<PC6)

#define BMASK (LED4 | LED5 | LED6 | LED7)
#define CMASK LEDPWR
#define DMASK (LED0 | LED1 | LED2 | LED3)

#define LED_SCANNER_DELAY 100000

unsigned long tLedScannerUpdate;
uint8_t brightness[9];
uint8_t ledScannerDir;
uint8_t ledScannerPos;

void LedInit() {
    uint8_t i;

    DDRB |= BMASK;
    DDRC |= CMASK;
    DDRD |= DMASK;

    tLedScannerUpdate = 0;
    ledScannerDir = LED_SCANNER_FORWARD;
    ledScannerPos = 0;

    for (i=0; i<8; i++) {
        SetLedBright(i, 255);
    }

    SetLedBright(8, 255);
}

void SetLedBright(uint8_t led, uint8_t value)
{
   brightness[led] = value;

   switch (led) {
       case 0: 
          PORTD = (PORTD & ~LED0) | (value ? 0 : LED0);
          break;
       case 1: 
          PORTD = (PORTD & ~LED1) | (value ? 0 : LED1);
          break;
       case 2: 
          PORTD = (PORTD & ~LED2) | (value ? 0 : LED2);
          break;
       case 3: 
          PORTD = (PORTD & ~LED3) | (value ? 0 : LED3);
          break;
       case 4: 
          PORTB = (PORTB & ~LED4) | (value ? 0 : LED4);
          break;
       case 5: 
          PORTB = (PORTB & ~LED5) | (value ? 0 : LED5);
          break;
       case 6: 
          PORTB = (PORTB & ~LED6) | (value ? 0 : LED6);
          break;
       case 7: 
          PORTB = (PORTB & ~LED7) | (value ? 0 : LED7);
          break;          
       case 8:
          PORTC = (PORTC & !LEDPWR) | (value ? LEDPWR : 0);
          break;         
   }
}

void LedScannerUpdate()
{
    if ((tLoopTop>=tLedScannerUpdate) && ((tLoopTop-tLedScannerUpdate)<LED_SCANNER_DELAY)) {
        return;
    }

    SetLedBright(ledScannerPos, 0);

    if (ledScannerDir == LED_SCANNER_FORWARD) {
        if (ledScannerPos == 7) {
            ledScannerPos = 6;
            ledScannerDir = LED_SCANNER_BACKWARD;
        } else {
            ledScannerPos++;
        }
    } else if (ledScannerDir == LED_SCANNER_BACKWARD) {
        if (ledScannerPos == 0) {
            ledScannerPos = 1;
            ledScannerDir = LED_SCANNER_FORWARD;
        } else {
            ledScannerPos --;
        }
    }

    tLedScannerUpdate = tLoopTop;
    SetLedBright(ledScannerPos, 255);
}


uint8_t GetLedBright(uint8_t led)
{
    return brightness[led];
}