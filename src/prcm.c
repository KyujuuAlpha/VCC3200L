#include "hw_types.h"
#include "prcm.h"
#include "vcc3200.h"

void PRCMCC3200MCUInit(void) {
    init();
}

void PRCMPeripheralClkEnable(unsigned long ulPeripheral, unsigned long ulClkFlags) {
    //dummy
}

unsigned long PRCMPeripheralClockGet(unsigned long ulPeripheral) {
    //dummy
    return 0;
}

void PRCMPeripheralReset(unsigned long ulPeripheral) {
    //dummy
}