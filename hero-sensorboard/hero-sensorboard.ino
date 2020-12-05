#include <Arduino.h>
#include <Wire.h>
#include "slave.h"
#include "globals.h"
#include "leds.h"
#include "keyboard.h"

unsigned long tLoopTop;

void setup()
{
    LedInit();
    KeyboardInit();
    //SlaveInit();
}

void loop()
{
    tLoopTop = micros();

    //LedScannerUpdate();
    KeyboardUpdate();
}