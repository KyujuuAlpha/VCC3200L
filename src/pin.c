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
    pins[ulPin] = (struct Pin) { .val = 0, .pinMode = ulPinMode, .gpio = (struct GPIO) { .port = 0, .pin = 0, .assigned = false } };
}

void PinTypeGPIO(unsigned long ulPin, unsigned long ulPinMode, tBoolean bOpenDrain) {
    PinModeSet(ulPin, ulPinMode);
    pins[ulPin].pinMode = PIN_MODE_1;
}

void PinTypeSPI(unsigned long ulPin, unsigned long ulPinMode) {
    PinModeSet(ulPin, ulPinMode);
    pins[ulPin].pinMode = PIN_MODE_2;
}

void PinTypeUART(unsigned long ulPin, unsigned long ulPinMode) {
    PinModeSet(ulPin, ulPinMode);
    pins[ulPin].pinMode = PIN_MODE_3;
}

void PinTypeI2C(unsigned long ulPin, unsigned long ulPinMode) {
    PinModeSet(ulPin, ulPinMode);
    pins[ulPin].pinMode = PIN_MODE_4;
}

void setPin(unsigned long ulPin, unsigned char val) {
    pins[ulPin].val = ulPin & val;
}

void setPinGPIO(unsigned long ulPort, unsigned char ucPin, unsigned char val) {
    for (int i = 0; i < PIN_64 + 1; i++) {
        if (pins[i].pinMode == PIN_MODE_1) {
            if (!pins[i].gpio.assigned) {
                pins[i].gpio.assigned = true;
                pins[i].gpio.port = ulPort;
                pins[i].gpio.pin = ucPin;
                setPin(i, val);
                printf("registered gpio pin\n");
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

unsigned char getPin(unsigned long ulPin) {
    return pins[ulPin].val;
}

unsigned char getPinGPIO(unsigned long ulPort, unsigned char ucPin) {
    for (int i = 0; i < PIN_64 + 1; i++) {
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