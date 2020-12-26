#include "ledModule.h"
#include <TM1638plus.h>

TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM, HIGH_FREQ);

void ledModule::init() {
    tm.displayBegin();
}

void ledModule::setLED(uint8_t position, uint8_t value) {
    tm.setLED(position, value);
}