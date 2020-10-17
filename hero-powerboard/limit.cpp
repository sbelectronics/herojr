#include <Arduino.h>
#include "limit.h"

// It would have made a whole lot of sense to put these all on the same port!!! 
#define LIMCW (1<<PC0)
#define LIMCCW (1<<PC1)

#define LMASK (LIMCW | LIMCCW)

void LimitInit() {
    // set as inputs and turn on pullups
    DDRC &= (~LMASK);
    PORTC |= LMASK;
}

bool LimitCWDown() {
    return ((PINC & LIMCW) == 0);
}

bool LimitCCWDown() {
    return ((PINC & LIMCCW) == 0);
}