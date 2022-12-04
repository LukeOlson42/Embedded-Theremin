#include "inc/LCD.h"

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

void DrawMenuOptions()
{
    for(uint8_t i = 0; i < MAX_MENU_OPTIONS; i++)
    {
        if(!MenuOptionsTable->MenuOptions[i])
        {
            DrawString(0, i, MenuOptionsTable->MenuOptions[i], 0xffff, 0x0000, 1);
        }
    }
}