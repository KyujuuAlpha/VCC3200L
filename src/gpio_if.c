#include "stdbool.h"
#include "gpio_if.h"

#include <stdio.h>

struct LED {
    unsigned char status;
};

static struct LED led1, led2, led3;

void GPIO_IF_LedConfigure(unsigned char ucPins) {
    if(ucPins & LED1) {
        led1 = (struct LED) { .status = 0 };
    }
    if(ucPins & LED2) {
        led2 = (struct LED) { .status = 0 };
    }
    if(ucPins & LED3) {
        led3 = (struct LED) { .status = 0 };
    }
}

void GPIO_IF_LedOn(char ledNum) {
    switch(ledNum) {
        case MCU_ON_IND:
        case MCU_EXECUTE_SUCCESS_IND:
        case MCU_GREEN_LED_GPIO: {
            led1.status = 1;
            break;
        }
        case MCU_SENDING_DATA_IND:
        case MCU_EXECUTE_FAIL_IND:
        case MCU_ORANGE_LED_GPIO: {
            led2.status = 1;
            break;
        }
        case MCU_ASSOCIATED_IND:
        case MCU_IP_ALLOC_IND:
        case MCU_SERVER_INIT_IND:
        case MCU_CLIENT_CONNECTED_IND:
        case MCU_RED_LED_GPIO: {
            led3.status = 1;
            break;
        }
        case MCU_ALL_LED_IND:
        {
            led1.status = 1;
            led2.status = 1;
            led3.status = 1;
            break;
        }
        default:
            break;
    }
}

void GPIO_IF_LedOff(char ledNum) {
    switch(ledNum) {
        case MCU_ON_IND:
        case MCU_EXECUTE_SUCCESS_IND:
        case MCU_GREEN_LED_GPIO: {
            led1.status = 0;
            break;
        }
        case MCU_SENDING_DATA_IND:
        case MCU_EXECUTE_FAIL_IND:
        case MCU_ORANGE_LED_GPIO: {
            led2.status = 0;
            break;
        }
        case MCU_ASSOCIATED_IND:
        case MCU_IP_ALLOC_IND:
        case MCU_SERVER_INIT_IND:
        case MCU_CLIENT_CONNECTED_IND:
        case MCU_RED_LED_GPIO: {
            led3.status = 0;
            break;
        }
        case MCU_ALL_LED_IND:
        {
            led1.status = 0;
            led2.status = 0;
            led3.status = 0;
            break;
        }
        default:
            break;
    }
}

unsigned char GPIO_IF_LedStatus(unsigned char ucGPIONum) {
    unsigned char ucLEDStatus;
    switch(ucGPIONum) {
    case MCU_GREEN_LED_GPIO: {
        ucLEDStatus = led1.status;
        break;
    }
    case MCU_ORANGE_LED_GPIO: {
        ucLEDStatus = led2.status;
        break;
    }
    case MCU_RED_LED_GPIO: {
        ucLEDStatus = led3.status;
        break;
        }
    default:
        ucLEDStatus = 0;
    }
    return ucLEDStatus;
}