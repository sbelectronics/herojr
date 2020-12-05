#ifndef __LEDS_H__
#define __LEDS_H__

#define LED_SCANNER_OFF 0
#define LED_SCANNER_FORWARD 1
#define LED_SCANNER_BACKWARD 2

void LedInit();
void LedScannerUpdate();
void SetLedBright(uint8_t led, uint8_t value);
uint8_t GetLedBright(uint8_t led);

#endif
