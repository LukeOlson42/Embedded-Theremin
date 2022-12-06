#ifndef LCD_H_
#define LCD_H_

#include "config.h"
#include "ST7735.h"
#include "System.h"
#include "I2C.h"

/*************** Data Structures ***************/
typedef struct DayTable_s {
    DayOfWeek day;
    char*     dayString;
} DayTable_s;

typedef struct MonthTable_s {
    MonthOfYear month;
    char*       monthString;
} MonthTable_s;

typedef struct LEDAndBorderTable_s {
    LEDAndBorderColor color;
    uint8_t LEDPinBitmask;
    uint16_t LCDColorData;
} LEDAndBorderTable_s;
/***************************************************/

typedef struct MenuOptionsTable_s {
    MenuState Menu;
    char* MenuOptions[MAX_MENU_OPTIONS];
    MenuState NextMenu[MAX_MENU_OPTIONS];
    bool LargeNumber;
} MenuOptionsTable_s;

typedef struct RTCDataLoadingTable_s {
    MenuState menu;
    RTC_Address address;
} RTCDataLoadingTable_s;


void LCDInit();

void DrawString(uint8_t x, uint8_t y, char *buf, uint16_t textColor, uint16_t bkgColor, uint8_t size);
void Clock_Init48MHz(void);

void ClearMenu();
void DrawMenuStructure(void);
void DrawMenuOptions(MenuState menu);
MenuState FindNextMenu(MenuState menu, uint8_t selection);

void DisplayVolumeBars(void);
void UpdateVolumeBars(void);

void DisplayRTCData(void);

void DrawBorders(void);

RTC_Address GetDesiredRTCAddress();








#endif