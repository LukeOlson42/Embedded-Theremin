#ifndef LCD_H_
#define LCD_H_

#include "config.h"
#include "ST7735.h"
#include "System.h"

/*************** Data Structures ***************/
typedef struct DayTable_s {
    DayOfWeek day;
    char*     dayString;
} DayTable_s;

typedef struct MonthTable_s {
    MonthOfYear month;
    char*       monthString;
} MonthTable_s;
/***************************************************/

typedef void (*MenuFunc)(void);

typedef struct MenuOptionsTable_s {
    MenuState Menu;
    char* MenuOptions[MAX_MENU_OPTIONS];
    MenuState NextMenu[MAX_MENU_OPTIONS];
    MenuFunc MenuOptionRoutine[MAX_MENU_OPTIONS];
} MenuOptionsTable_s;



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

// static char* GetDayString(DayOfWeek day);
// static char* GetMonthString(MonthOfYear month);








#endif