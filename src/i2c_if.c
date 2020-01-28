#include <stdio.h>
#include "i2c_if.h"
#include "config.h"

struct Register {
    char id;
    int data;
};

struct Device {
    char id, reg;
    struct Register registers[I2C_REG_COUNT];
};

static struct Device devices[I2C_DEV_COUNT];

int I2C_IF_Open(unsigned long ulMode) {
    if (ENABLE_DEBUG) {
        printf("Enabled I2C interface\n");
    }
    for (int i = 0; i < I2C_DEV_COUNT; i++) {
        devices[i] = (struct Device) { .id = -1, .reg = 0, .registers = {} };
        for (int j = 0; j < I2C_REG_COUNT; j++) {
            devices[i].registers[j] = (struct Register) { .id = -1, .data = 0};
        }
    }
}

int I2C_IF_Close() {
    for (int i = 0; i < I2C_DEV_COUNT; i++) {
        devices[i].id = -1;
        for (int j = 0; j < I2C_REG_COUNT; j++) {
            devices[i].registers[j].id = -1;
        }
    }
}

int I2C_IF_Write(unsigned char ucDevAddr, unsigned char *pucData, unsigned char ucLen, unsigned char ucStop) {
    for (int i = 0; i < I2C_DEV_COUNT; i++) {
        if (devices[i].id == ucDevAddr) {
            devices[i].reg = *pucData;
        }
    }
}

int I2C_IF_Read(unsigned char ucDevAddr, unsigned char *pucData, unsigned char ucLen) {
    for (int i = 0; i < I2C_DEV_COUNT; i++) {
        if (devices[i].id == ucDevAddr) {
            int find = devices[i].reg;
            for (int j = 0; j < I2C_REG_COUNT; j++) {
                if(devices[i].registers[j].id == find) {
                    *pucData = devices[i].registers[j].data;
                    return 1;
                }
            }
        }
    }
    *pucData = 0;
    return 0;
}

void writeRegister(unsigned char dev, unsigned char reg, int data) {
    for (int i = 0; i < I2C_DEV_COUNT; i++) {
        if (devices[i].id == dev) {
            for (int j = 0; j < I2C_REG_COUNT; j++) {
                if(devices[i].registers[j].id == reg) {
                    devices[i].registers[j].data = data;
                    return;
                }
            }
            for (int j = 0; j < I2C_REG_COUNT; j++) {
                if(devices[i].registers[j].id == -1) {
                    devices[i].registers[j].id = reg;
                    devices[i].registers[j].data = data;
                    return;
                }
            }
        }
    }
    for (int i = 0; i < I2C_DEV_COUNT; i++) {
        if (devices[i].id == -1) {
            devices[i].id = dev;
            devices[i].registers[0].id = reg;
            devices[i].registers[0].data = data;
            return;
        }
    }
}