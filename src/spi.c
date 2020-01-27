#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>

#include "hw_types.h"
#include "pin.h"
#include "spi.h"
#include "stdbool.h"
#include "config.h"

struct PinEntry {
    int pin;
    unsigned char val;
};

struct SPIEntry {
    bool CS;
    struct PinEntry pins[SPI_MAX_PINS];
    struct SPIEntry *next;
};

static struct SPIEntry *next = NULL, *current = NULL;
static int clkPin = -1, mosiPin = -1;

void SPIConfigSetExpClk(unsigned long ulBase,unsigned long ulSPIClk,
            unsigned long ulBitRate, unsigned long ulMode,
            unsigned long ulSubMode, unsigned long ulConfig) {
                
            }

void SPIEnable(unsigned long ulBase) {
    bool flag = false;
    for (int i = PIN_01; i <= PIN_64; i++) {
        if (getPinMode(i) == PIN_MODE_2) {
            if (!flag) {
                clkPin = i;
                flag = true;
            } else {
                mosiPin = i;
                return;
            }
        }
    }
}

void SPIDisable(unsigned long ulBase) {
    clkPin = -1;
    mosiPin = -1;
    SPIReset(ulBase);
}

void SPIReset(unsigned long ulBase) {
    struct SPIEntry *entry;
    while (next != NULL) {
        entry = next->next; //Free up the spi entries
        next = next->next;
        free(entry);
    }
    current = NULL;
}

void SPIDataGet(unsigned long ulBase, unsigned long *pulData) {
    //dummy
}

void SPIDataPut(unsigned long ulBase, unsigned long ulData) {
    setPinSPI(mosiPin, ulData);
    usleep(50);
}

void SPICSEnable(unsigned long ulBase) {
    if (current == NULL) {
        current = (struct SPIEntry*) malloc(sizeof(struct SPIEntry));
        for (int i = 0; i < SPI_MAX_PINS; i++) {
            current->pins[i] = (struct PinEntry) { .pin = -1, .val = 0 };
        }
        current->CS = true;
        current->next = NULL;
        for (int i = PIN_01; i <= PIN_64; i++) {
            if (getPin(i)) {
                spiAdd(i | PIN_FLAG, getPin(i));
            }
        }
        if (next == NULL) {
            next = current;
        } else {
            struct SPIEntry *add = next;
            while (add != NULL) {
                if (add->next == NULL) {
                    add->next = current;
                    return;
                } else {
                    add = add->next;
                }
            }
        }
    }
}

void SPICSDisable(unsigned long ulBase) {
    if (current != NULL) {
        current->CS = false;
        current = NULL;
    }
}

void spiAdd(unsigned long ulPin, unsigned char val) {
    if (current != NULL) {
        if(current->CS) {
            for (int i = 0; i < SPI_MAX_PINS; i++) {
                if(current->pins[i].pin == -1) {
                    current->pins[i].pin = ulPin;
                    current->pins[i].val = val;
                    break;
                }
            }
        }
    }
}

unsigned char processPinEntry(unsigned long *ulPin, unsigned char *val) {
    if (next != NULL && !next->CS) {
        for (int i = 0; i < SPI_MAX_PINS; i++) {
            if(next->pins[i].pin != -1) {
                *ulPin = next->pins[i].pin;
                *val = next->pins[i].val;
                next->pins[i].pin = -1;
                next->pins[i].val = 0;
                return true;
            }
        }
        struct SPIEntry *entry = next;
        next = next->next;
        free(entry);
    }
    return false;
}

long SPITransfer(unsigned long ulBase, unsigned char *ucDout,
                   unsigned char *ucDin, unsigned long ulSize,
                   unsigned long ulFlags) {

}