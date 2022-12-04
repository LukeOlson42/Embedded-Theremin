#ifndef LCD_H_
#define LCD_H_

#include "config.h"
#include "ST7735.h"
#include "System.h"


typedef struct MenuOptionsTable_s {
    MenuState Menu;
    char* MenuOptions[MAX_MENU_OPTIONS];
} MenuOptionsTable_s;

static const MenuOptionsTable_s MenuOptionsTable[] = {
    {Main, {"test", NULL, NULL, NULL}},
};



void LCDInit();

void DrawString(uint8_t x, uint8_t y, char *buf, uint16_t textColor, uint16_t bkgColor, uint8_t size);
void Clock_Init48MHz(void);


void DrawMenuOptions();











#endif