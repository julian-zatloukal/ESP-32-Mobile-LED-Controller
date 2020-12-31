#include "ledModule.h"
#include <TM1638plus.h>

TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM, HIGH_FREQ);

uint8_t ledArrayState[8] = {0};

void ledModule::init() {
    tm.displayBegin();
}

void ledModule::setLED(uint8_t position, uint8_t value) {
    tm.setLED(position, value);
    ledArrayState[position] = value;
}

uint8_t* ledModule::getLedStateArray() {
    return ledArrayState;
}