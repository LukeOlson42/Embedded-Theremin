#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "msp.h"

// add any defines

#define CLOCK_SPEED_MHZ 48
#define CYCLES_PER_MS (CLOCK_SPEED_MHZ * 1000)

#define PERIOD_TO_INCHES_RATIO 0.0089

#define MAX_MENU_OPTIONS 4
#define OPTIONS_TABLE_SIZE 4
#define MENU_Y_OFFSET 9

#define DATE_TIME_MENU_Y_OFFSET 3

#define LARGE_MENU_Y_OFFSET 3
#define LARGE_MENU_X_OFFSET 6

#define VOLUME_BARS_X 15
#define VOLUME_BARS_Y 100
#define VOLUME_BARS_WIDTH 5
#define VOLUME_BARS_HEIGHT 40
#define VOLUME_BARS_SEPARATION 5





#endif