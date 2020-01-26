#include "hw_types.h"
#include "spi.h"

void SPIConfigSetExpClk(unsigned long ulBase,unsigned long ulSPIClk,
            unsigned long ulBitRate, unsigned long ulMode,
            unsigned long ulSubMode, unsigned long ulConfig) {
                
            }

void SPIEnable(unsigned long ulBase) {

}

extern void SPIDisable(unsigned long ulBase) {

}

extern void SPIReset(unsigned long ulBase) {

}

void SPIDataGet(unsigned long ulBase, unsigned long *pulData) {

}

void SPIDataPut(unsigned long ulBase, unsigned long ulData) {

}

void SPICSEnable(unsigned long ulBase) {

}

void SPICSDisable(unsigned long ulBase) {

}

long SPITransfer(unsigned long ulBase, unsigned char *ucDout,
                   unsigned char *ucDin, unsigned long ulSize,
                   unsigned long ulFlags) {

                   }