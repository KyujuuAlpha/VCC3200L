#include "stdbool.h"
#include "hw_types.h"
#include "pin.h"

#include <stdio.h>

struct GPIO {
    unsigned long port;
    unsigned char pin;
    bool assigned;
};

struct Pin {
    unsigned char val;
    unsigned long pinMode; 
    struct GPIO gpio;
};

static struct Pin pins[PIN_64 + 1];

void PinModeSet(unsigned long ulPin, unsigned long ulPinMode) {
    pins[(int) ulPin] = (struct Pin) { .val = 0, .pinMode = ulPinMode, .gpio = (struct GPIO) { .port = 0, .pin = 0, .assigned = false } };
}

void PinTypeGPIO(unsigned long ulPin, unsigned long ulPinMode, tBoolean bOpenDrain) {
    PinModeSet(ulPin, ulPinMode);
    pins[(int) ulPin].pinMode = PIN_MODE_1;  //redefine pin mode 1 to GPIO
}

void PinTypeSPI(unsigned long ulPin, unsigned long ulPinMode) {
    PinModeSet(ulPin, ulPinMode);
    pins[(int) ulPin].pinMode = PIN_MODE_2; //redefine pin mode 2 to SPI
}

void PinTypeUART(unsigned long ulPin, unsigned long ulPinMode) {
    PinModeSet(ulPin, ulPinMode);
    pins[(int) ulPin].pinMode = PIN_MODE_3; //redefine pin mode 3 to UART
}

void PinTypeI2C(unsigned long ulPin, unsigned long ulPinMode) {
    PinModeSet(ulPin, ulPinMode);
    pins[(int) ulPin].pinMode = PIN_MODE_4; //redefine pin mode 4 to i2c
}

void setPin(unsigned long ulPin, unsigned char val) {
    pins[(int) ulPin].val = ulPin & val;
    spiAdd(ulPin, pins[ulPin].val);
}

void setPinGPIO(unsigned long ulPort, unsigned char ucPin, unsigned char val) {
    for (int i = 0; i <= PIN_64; i++) {
        if (pins[i].pinMode == PIN_MODE_1) {
            if (!pins[i].gpio.assigned) { //a little hacky, but it works for now
                pins[i].gpio.assigned = true;
                pins[i].gpio.port = ulPort;
                pins[i].gpio.pin = ucPin;
                setPin(i, val);
                return;
            } else {
                if (pins[i].gpio.port == ulPort && pins[i].gpio.pin == ucPin) {
                    setPin(i, val);
                    return;
                }
            }
        }
    }
}

void setPinSPI(unsigned long ulPin, unsigned char val) {
    pins[(int) ulPin].val = val;
    spiAdd(ulPin, pins[ulPin].val);
}

unsigned char getPin(unsigned long ulPin) {
    return pins[(int) ulPin].val;
}

unsigned long getPinMode(unsigned long ulPin) {
    return pins[(int) ulPin].pinMode;
}

unsigned char getPinGPIO(unsigned long ulPort, unsigned char ucPin) {
    for (int i = 0; i <= PIN_64; i++) {
        if (pins[i].pinMode == PIN_MODE_1) {
            if (pins[i].gpio.assigned) {
                if (pins[i].gpio.port == ulPort && pins[i].gpio.pin == ucPin) {
                    return getPin(i);
                }
            }
        }
    }
    return 0;
}