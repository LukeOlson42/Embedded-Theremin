#ifndef LCD_H_
#define LCD_H_

#include "config.h"
#include "ST7735.h"
#include "System.h"

// /*************** Data for Day Strings ***************/
// typedef struct DayTable_s {
//     DayOfWeek day;
//     char*     dayString;
// } DayTable_s;

// static const DayTable_s DayTable[] = {
//     {Monday,       "Monday"}, {Tuesday,   "Tuesday"}, 
//     {Wednesday, "Wednesday"}, {Thursday, "Thursday"}, 
//     {Friday,       "Friday"}, {Saturday, "Saturday"}, 
//     {Sunday,       "Sunday"}, 
// };
// /***************************************************/


// /************** Data for Month Strings *************/
// typedef struct MonthTable_s {
//     MonthOfYear month;
//     char*       monthString;
// };

// static const MonthTable_s MonthTable[] = {
//     {January,     "January"}, {February, "February"}, 
//     {March,         "March"}, {April,       "April"}, 
//     {May,             "May"}, {June,         "June"}, 
//     {July,           "July"}, {August,     "August"}, 
//     {September, "September"}, {October,   "October"}, 
//     {November,   "November"}, {December, "December"}, 
// };
// /***************************************************/

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











#endif