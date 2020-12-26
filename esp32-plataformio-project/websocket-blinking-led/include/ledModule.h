#pragma once
#ifndef LED_MODULE_H_
#define LED_MODULE_H_

/* https://github.com/gavinlyonsrepo/TM1638plus */
#include <stdint.h>

// TM1638 module configuration
#define STROBE_TM 4    // strobe = GPIO connected to strobe line of module
#define CLOCK_TM 16    // clock = GPIO connected to clock line of module
#define DIO_TM 17      // data = GPIO connected to data line of module
#define HIGH_FREQ true // Required for ESP32

// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)

class ledModule{
    public:
        static void init();
        static void setLED(uint8_t position, uint8_t value);

};

#endif 
