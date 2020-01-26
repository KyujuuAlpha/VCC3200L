#include <stdio.h>

#include "hw_types.h"
#include "uart.h"

long UARTCharGetNonBlocking(unsigned long ulBase) {
    return getchar();
}

void UARTCharPut(unsigned long ulBase, unsigned char ucData){
    printf("%c", ucData);
}

tBoolean UARTCharsAvail(unsigned long ulBase){
    return true;
}

void UARTConfigSetExpClk(unsigned long ulBase, unsigned long ulUARTClk, unsigned long ulBaud, unsigned long ulConfig) {
    // dummy
}

void UARTConfigGetExpClk(unsigned long ulBase, unsigned long ulUARTClk, unsigned long *pulBaud, unsigned long *pulConfig) {
    // dummy 
}