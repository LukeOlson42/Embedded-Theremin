#include "inc/LCD.h"

/*****************Menu Options Table***********************************/
static const MenuOptionsTable_s MenuOptionsTable[OPTIONS_TABLE_SIZE] = {

    {Main, 
    {"Update Time", "Display Note", "Display Volume", NULL}, 
    {TimeChange, PitchDisplay, VolumeDisplay, NullMenu}, 
    {NULL, NULL, NULL, NULL}},


    {TimeChange, 
    {"Update Time", "Update Date", "Back", NULL}, 
    {SystemStateChange, SystemStateChange, Back, NullMenu}, 
    {NULL, NULL, NULL, NULL}},


    {PitchDisplay, 
    {NULL, NULL, NULL, "Back"}, 
    {NullMenu, NullMenu, NullMenu, Back}, 
    {NULL, NULL, NULL, NULL}},

    {VolumeDisplay, 
    {NULL, NULL, NULL, "Back"}, 
    {NullMenu, NullMenu, NullMenu, Back}, 
    {NULL, NULL, NULL, NULL}},

};
/**********************************************************************/

static const DayTable_s DayTable[] = {
    {Monday,       "Monday"}, {Tuesday,   "Tuesday"}, 
    {Wednesday, "Wednesday"}, {Thursday, "Thursday"}, 
    {Friday,       "Friday"}, {Saturday, "Saturday"}, 
    {Sunday,       "Sunday"}, 
};

static const MonthTable_s MonthTable[] = {
    {January,     "January"}, {February, "February"}, 
    {March,         "March"}, {April,       "April"}, 
    {May,             "May"}, {June,         "June"}, 
    {July,           "July"}, {August,     "August"}, 
    {September, "September"}, {October,   "October"}, 
    {November,   "November"}, {December, "December"}, 
};

void DrawString(uint8_t x, uint8_t y, char *buf, uint16_t textColor, uint16_t bkgColor, uint8_t size)
{
    if(y>15) return;

    for(uint8_t i = 0; i < strlen(buf); i++)
    {
        if(x > 20)
        {
            x = 0;
            y += size;
        }

        if(buf[i] == '\n')
        {
            y += size;
        }
        else if(buf[i] == '\r')
        {
            x = 0;
        }
        else
        {
            ST7735_DrawCharS(x*6, y*10, buf[i], textColor, bkgColor, size);
            x += size;
        }
    }
}

void Clock_Init48MHz(void)
{
    
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | // unlocks memory banks
    FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
    FLCTL_BANK1_RDCTL_WAIT_1;


    PJ->SEL0 |= BIT2 | BIT3;
    PJ->SEL1 &= ~(BIT2 | BIT3);

    CS->KEY = CS_KEY_VAL;                       // unlocks clock module
    CS->CTL2 |= CS_CTL2_HFXT_EN | CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE; // enables HFXT

    while(CS->IFG & CS_IFG_HFXTIFG)
    {
        CS->CLRIFG |= CS_CLRIFG_CLR_HFXTIFG;
    }
                

    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK | CS_CTL1_SELS_MASK | // sets MCLK to 48MHz
                            CS_CTL1_DIVHS_MASK) | CS_CTL1_SELM__HFXTCLK | CS_CTL1_SELS__HFXTCLK;

    CS->CTL1 = CS->CTL1 | 0x40000000;    // sets SMCLK to 3 MHZ

    CS->KEY = 0; 
}

void LCDInit(void)
{
    Clock_Init48MHz();
    ST7735_InitR(INITR_REDTAB);
}


void DrawMenuStructure(void)
{
    ST7735_DrawFastHLine(0, 80, ST7735_TFTWIDTH, 0xffff);
}


void DrawMenuOptions(MenuState menu)
{
    char OptionStringHeader[3] = "1)";

    for(uint8_t i = 0; i < OPTIONS_TABLE_SIZE; i++)
    {
        if(MenuOptionsTable[i].Menu == menu)
        {
            for(uint8_t j = 0; j < MAX_MENU_OPTIONS; j++)
            {
                if(MenuOptionsTable[i].MenuOptions[j] != NULL)
                {
                    uint8_t xOffset = ((Theremin.Flags.LargeNumberMenu) ? LARGE_MENU_X_OFFSET : 0);
                    uint8_t yOffset = ((Theremin.Flags.LargeNumberMenu) ? LARGE_MENU_Y_OFFSET : 0);

                    DrawString(0 + xOffset, j + MENU_Y_OFFSET + yOffset, OptionStringHeader, 0xffff, 0x0000, 1);
                    DrawString(3 + xOffset, j + MENU_Y_OFFSET + yOffset, MenuOptionsTable[i].MenuOptions[j], 0xffff, 0x0000, 1);
                }
                OptionStringHeader[0]++;
            }
        }
    }
}

MenuState FindNextMenu(MenuState menu, uint8_t selection)
{
    for(uint8_t i = 0; i < OPTIONS_TABLE_SIZE; i++)
    {
        if(MenuOptionsTable[i].Menu == menu)
        {
            return MenuOptionsTable[i].NextMenu[selection - 1];
        }
    }

    return NullMenu;
}

void ClearMenu(void)
{
    ST7735_FillRect(0, ST7735_TFTHEIGHT / 2 + 1, ST7735_TFTWIDTH, ST7735_TFTHEIGHT / 2, 0x0000); // +1 offset prevents erasure of horizontal line
}

void DisplayVolumeBars()
{
    for(uint8_t i = 0; i < Theremin.Speaker.DiscreteVolume; i++)
    {
        ST7735_FillRect(VOLUME_BARS_X + (VOLUME_BARS_WIDTH + 5) * i, VOLUME_BARS_Y, VOLUME_BARS_WIDTH, VOLUME_BARS_HEIGHT, 0xffff);
    }
}

void UpdateVolumeBars()
{
    if(Theremin.Menu == VolumeDisplay)
    {
        uint8_t BarPosition = Theremin.Speaker.DiscreteVolume - 1;

        if(Theremin.Flags.VolumeDown)
        {
            BarPosition++;
            ST7735_FillRect(VOLUME_BARS_X + (VOLUME_BARS_WIDTH + VOLUME_BARS_SEPARATION) * BarPosition, VOLUME_BARS_Y, VOLUME_BARS_WIDTH, VOLUME_BARS_HEIGHT, 0x0000);
            Theremin.Flags.VolumeDown = false;
        }
        if(Theremin.Flags.VolumeUp)
        {
            ST7735_FillRect(VOLUME_BARS_X + (VOLUME_BARS_WIDTH + VOLUME_BARS_SEPARATION) * BarPosition, VOLUME_BARS_Y, VOLUME_BARS_WIDTH, VOLUME_BARS_HEIGHT, 0xffff);
            Theremin.Flags.VolumeUp = false;

            P1->OUT |= BIT0;
        }
    }
}

static char* GetDayString(DayOfWeek day)
{
    for(uint8_t i = Sunday; i <= Saturday; i++)
    {
        if(DayTable[i].day == day)
        {
            return DayTable[i].dayString;
        }
    }

    return NULL;
}

static char* GetMonthString(MonthOfYear month)
{
    for(uint8_t i = January; i <= December; i++)
    {
        if(MonthTable[i].month == month)
        {
            return MonthTable[i].monthString;
        }
    }

    return NULL;
}

void DisplayRTCData(void)
{
    char* MonthStr = GetMonthString(Theremin.RTC.CalendarDate.Month);
    char* DayStr = GetDayString(Theremin.RTC.CalendarDate.Day);

    if(MonthStr)
    {
        DrawString(6, 0, MonthStr, 0xffff, 0x0000, 1);
    }

    if(DayStr)
    {
        DrawString(4, 1, DayStr, 0xffff, 0x0000, 1);
    }
}


