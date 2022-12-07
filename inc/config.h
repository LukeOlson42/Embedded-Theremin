#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "msp.h"

// add any defines

#define CLOCK_SPEED_MHZ 48
#define CYCLES_PER_MS (CLOCK_SPEED_MHZ * 1000)

#define PERIOD_TO_INCHES_RATIO 0.0089

#define MAX_MENU_OPTIONS 5
#define OPTIONS_TABLE_SIZE 13
#define MENU_Y_OFFSET 9

#define DATE_TIME_MENU_Y_OFFSET 3

#define LARGE_MENU_Y_OFFSET 3
#define LARGE_MENU_X_OFFSET 6

#define VOLUME_BARS_X 15
#define VOLUME_BARS_Y 100
#define VOLUME_BARS_WIDTH 5
#define VOLUME_BARS_HEIGHT 40
#define VOLUME_BARS_SEPARATION 5

#define BASE_NUMBER_INPUT_OFFSET_X 20
#define BASE_NUMBER_INPUT_OFFSET_Y 120

#define RTC_PERIPH_ADD 0b1101000 // peripheral address
#define EEPROM_ADDR 0b01010001

#define MOTOR_STEPS_PER_VOLUME_STEP 4


#endif