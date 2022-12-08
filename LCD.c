#include "inc/LCD.h"

/*****************Menu Options Table***********************************/
static const MenuOptionsTable_s MenuOptionsTable[OPTIONS_TABLE_SIZE] = {

    {Main, 
    {"Update RTC", "Display Note", "Display Volume", NULL}, 
    {RTCChange, PitchDisplay, VolumeDisplay, NullMenu},
    false},

    {RTCChange, 
    {"Update Time", "Update Date", "Back", NULL}, 
    {UpdateTime, UpdateDate, Main, NullMenu},
    false},

    {UpdateTime, 
    {"Update Seconds", "Update Minutes", "Update Hours", "Back"}, 
    {InputSeconds, InputMinutes, InputHours, RTCChange},
    false},

    {UpdateDate, 
    {"Update Month", "Update Day", "Update Date", "Update Year", "Back"}, 
    {InputMonth, InputDay, InputDate, InputYear, RTCChange},
    false},

    {PitchDisplay, 
    {NULL, NULL, NULL, "Back"}, 
    {NullMenu, NullMenu, NullMenu, Main},
    true},

    {VolumeDisplay, 
    {NULL, NULL, NULL, "Back"}, 
    {NullMenu, NullMenu, NullMenu, Main},
    true,},

    {InputDay, 
    {NULL, NULL, "Confirm", "Back"}, 
    {NullMenu, NullMenu, NullMenu, UpdateDate},
    true},

    {InputDate, 
    {NULL, NULL, "Confirm", "Back"}, 
    {NullMenu, NullMenu, NullMenu, UpdateDate},
    true},

    {InputMonth, 
    {NULL, NULL, "Confirm", "Back"}, 
    {NullMenu, NullMenu, NullMenu, UpdateDate},
    true},

    {InputYear, 
    {NULL, NULL, "Confirm", "Back"}, 
    {NullMenu, NullMenu, NullMenu, UpdateDate}, 
    true},

    {InputSeconds, 
    {NULL, NULL, "Confirm", "Back"}, 
    {NullMenu, NullMenu, NullMenu, UpdateTime}, 
    true},

    {InputMinutes, 
    {NULL, NULL, "Confirm", "Back"}, 
    {NullMenu, NullMenu, NullMenu, UpdateTime}, 
    true},

    {InputHours, 
    {NULL, NULL, "Confirm", "Back"}, 
    {NullMenu, NullMenu, NullMenu, UpdateTime}, 
    true},

};
/**********************************************************************/

/******************Day and Month String Data*******************/
static const DayTable_s DayTable[] = {
    {Monday,    "   Monday"}, {Tuesday,  "  Tuesday"}, 
    {Wednesday, "Wednesday"}, {Thursday, " Thursday"}, 
    {Friday,    "   Friday"}, {Saturday, " Saturday"}, 
    {Sunday,    "   Sunday"}, 
};

static const MonthTable_s MonthTable[] = {
    {January,   "  January"}, {February,  " February"}, 
    {March,     "    March"}, {April,     "    April"}, 
    {May,       "      May"}, {June,      "     June"}, 
    {July,      "     July"}, {August,    "   August"}, 
    {September, "September"}, {October,   "  October"}, 
    {November,  " November"}, {December,  " December"}, 
};

static const KeyStringTable_s KeyStringTable[] = {
    {A, "A "}, {Bb, "Bb"}, {B, "B "}, {C, "C "},
    {Db, "Db"}, {D, "D "}, {Eb, "Eb"}, {E, "E "},
    {F, "F "}, {Gb, "G "}, {G, "G "}, {Ab, "Ab"},
};
/*************************************************************/

/************LED and Border Table**************/
static const LEDAndBorderTable_s LEDAndBorderTable[] = {
    {Red,    0x20, ST7735_RED},
    {White,  0xE0, ST7735_WHITE},
    {Green,  0x40, ST7735_GREEN},
    {Aqua,   0xC0, ST7735_CYAN},
    {Blue,   0x80, ST7735_BLUE},
    {Violet, 0xA0, ST7735_MAGENTA},
    {Yellow, 0x60, ST7735_YELLOW},
    {Red,    0x20, ST7735_RED}
};
/**********************************************/

static const RTCDataLoadingTable_s RTCDataLoadingTable[] = {
    {    InputDay,     Day}, {InputDate, Date}, {InputMonth,   Month}, { InputYear,  Year},
    {InputSeconds, Seconds}, {InputMinutes, Minutes}, {InputHours, Hours}
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
    if(Theremin.State == DataInput)
    {
        strcpy(OptionStringHeader, "#)");
    }

    for(uint8_t i = 0; i < OPTIONS_TABLE_SIZE; i++)
    {
        if(MenuOptionsTable[i].Menu == menu)
        {
            for(uint8_t j = 0; j < MAX_MENU_OPTIONS; j++)
            {
                if(Theremin.State == DataInput)
                {
                   if(j == 3)
                    {
                        OptionStringHeader[0] = '*';
                    }
                }
                if(MenuOptionsTable[i].MenuOptions[j] != NULL)
                {
                    uint8_t xOffset = ((MenuOptionsTable[i].LargeNumber) ? LARGE_MENU_X_OFFSET : 0);
                    uint8_t yOffset = ((MenuOptionsTable[i].LargeNumber) ? LARGE_MENU_Y_OFFSET : 0);

                    DrawString(1 + xOffset, j + MENU_Y_OFFSET + yOffset, OptionStringHeader, 0xffff, 0x0000, 1);
                    DrawString(4 + xOffset, j + MENU_Y_OFFSET + yOffset, MenuOptionsTable[i].MenuOptions[j], 0xffff, 0x0000, 1);
                }
                if(Theremin.State != DataInput)
                {
                    OptionStringHeader[0]++;
                }
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

void ClearDate(void)
{
    ST7735_FillRect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT / 2 - 1, 0x0000); // +1 offset prevents erasure of horizontal line
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
        }
    }
}

void UpdateDrawnPitch()
{
    if(Theremin.Menu == PitchDisplay)
    {
        for(uint8_t i = 0; i < 12; i++)
        {
            if(KeyStringTable[i].key == Theremin.Speaker.NoteRelativeToKey)
            {
                DrawString(9, 10, KeyStringTable[i].string, 0xffff, 0x0000, 4);
            }
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

static void ConvertDateToString(char* dateString)
{
    uint8_t date = Theremin.RTC.CalendarDate.Date;
    uint16_t year = Theremin.RTC.CalendarDate.Year;

    char tempString[] = {
        '0' + date / 10,
        '0' + date % 10,
        ',',
        ' ',
        '2',
        '0',
        '0' + (year / 10) % 10,
        '0' + year % 10,
        '\0'
    };

    if(tempString[4] == '1')
    {
        P1->OUT |= BIT0;
    }

    strcpy(dateString, tempString);
}


static void ConvertTimeToString(char* timeString)
{
    uint8_t hour = Theremin.RTC.Time.Hour;
    uint8_t minute = Theremin.RTC.Time.Minute;
    uint8_t seconds = Theremin.RTC.Time.Second;

    char tempString[] = {
        '0' + hour / 10,
        '0' + hour % 10,
        ':',
        '0' + minute / 10,
        '0' + minute % 10,
        ':',
        '0' + seconds / 10,
        '0' + seconds % 10,
        '\0',
    };

    strcpy(timeString, tempString);
}

void DisplayRTCData(void)
{
    char* MonthStr = GetMonthString(Theremin.RTC.CalendarDate.Month);
    char* DayStr = GetDayString(Theremin.RTC.CalendarDate.Day);
    char DateStr[] = "00, 2000";
    char TimeStr[] = "00:00:00";

    ConvertDateToString(DateStr);
    ConvertTimeToString(TimeStr);

    if(MonthStr)
    {
        DrawString(1, DATE_TIME_MENU_Y_OFFSET, MonthStr, 0xffff, 0x0000, 1);
    }

    DrawString(11, DATE_TIME_MENU_Y_OFFSET, DateStr, 0xffff, 0x0000, 1);


    if(DayStr)
    {
        DrawString(2, DATE_TIME_MENU_Y_OFFSET + 1, DayStr, 0xffff, 0x0000, 1);
    }

    DrawString(12, DATE_TIME_MENU_Y_OFFSET + 1, TimeStr, 0xffff, 0x0000, 1);
}


static uint16_t GetDesiredColor(void)
{
    for(uint8_t i = 0; i <= 7; i++)
    {
        if(Theremin.Speaker.CurrentNote == i)
        {
            P8->OUT = LEDAndBorderTable[i].LEDPinBitmask >> 3;
            return LEDAndBorderTable[i].LCDColorData;
        }
    }

    return 0x0000;
}


void InitLEDs(void)
{
    P8->SEL0 &= ~(0xE0 >> 3);
    P8->SEL1 &= ~(0xE0 >> 3);
    P8->DIR |= (0xE0 >> 3);
}

void DrawBorders(void)
{
    uint16_t borderColor = GetDesiredColor();

    if(Theremin.Speaker.SensorDistanceInches > 18 || Theremin.Speaker.SensorDistanceInches < 3)
    {
        borderColor = 0x0000;
        P8->OUT = 0;
    }

    ST7735_FillRect(0, 0, 2, ST7735_TFTHEIGHT, borderColor);
    ST7735_FillRect(0, 0, ST7735_TFTWIDTH, 2, borderColor);
    ST7735_FillRect(ST7735_TFTWIDTH - 2, 0, 2, ST7735_TFTHEIGHT, borderColor);
    ST7735_FillRect(0, ST7735_TFTHEIGHT - 2, ST7735_TFTWIDTH, 2, borderColor);
}

RTC_Address GetDesiredRTCAddress()
{
    for(uint8_t i = 0; i < RTCEnd; i++)
    {
        if(RTCDataLoadingTable[i].menu == Theremin.Menu)
        {
            return RTCDataLoadingTable[i].address;
        }
    }

    return Seconds;
}