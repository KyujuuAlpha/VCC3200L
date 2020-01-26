#include "hw_types.h"
#include "pin.h"
#include "gpio.h"

void GPIODirModeSet(unsigned long ulPort, unsigned char ucPins, unsigned long ulPinIO) {
    setPinGPIO(ulPort, ucPins, 0);
}

long GPIOPinRead(unsigned long ulPort, unsigned char ucPins) {
    return getPinGPIO(ulPort, ucPins);
}

void GPIOPinWrite(unsigned long ulPort, unsigned char ucPins, unsigned char ucVal) {
    setPinGPIO(ulPort, ucPins, ucVal);
}