#ifndef CONFIG_H_
#define CONFIG_H_

#define ENABLE_GUI 1

#define ENABLE_SWITCHES 1
#define SWITCH_DELAY    250

#define SW2_PIN PIN_15
#define SW3_PIN PIN_04

#define ENABLE_LEDS 1
#define LED_TIMEOUT 100

#define ENABLE_OLED 0
#define OLED_WIDTH 128
#define OLED_HEIGHT 128
#define OLED_TIMEOUT 16

#define OLED_CLK  PIN_05
#define OLED_MOSI PIN_07
#define OLED_DC   PIN_18
#define OLED_R    PIN_08
#define OLED_OC   PIN_06

#define ENABLE_ACCEL 0
#define ACCEL_DEV 0x18
#define ACCEL_X_REG 0x3
#define ACCEL_Y_REG 0x5
#define ACCEL_Z_REG 0x7

#define SPI_MAX_PINS 7
#define PIN_FLAG 64

#define I2C_DEV_COUNT 5
#define I2C_REG_COUNT 5

#define ENABLE_DEBUG 0

#endif