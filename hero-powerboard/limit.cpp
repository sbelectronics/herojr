#include <Arduino.h>
#include "limit.h"

// It would have made a whole lot of sense to put these all on the same port!!! 
#define LIMCW (1<<PA0)
#define LIMCCW (1<<PA1)

#define LMASK (LIMCW | LIMCCW)

void LimitInit() {
    // set as inputs and turn on pullups
    DDRA &= (~LMASK);
    PORTA |= LMASK;
}

bool LimitCWDown() {
    return ((PINA & LIMCW) == 0);
}

bool LimitCCWDown() {
    return ((PINA & LIMCCW) == 0);
}