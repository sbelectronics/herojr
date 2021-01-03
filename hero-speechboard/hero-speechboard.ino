#include <Arduino.h>
#include <Wire.h>
#include "slave.h"
#include "speech.h"
#include "globals.h"

unsigned long tLoopTop;

void setup()
{
    SpeechInit();
    SlaveInit();

    SpeechAmpEnable(true);
    SpeechTest();
}

void loop()
{
    tLoopTop = micros();

    SpeechUpdate();
}