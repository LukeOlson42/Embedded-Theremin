#include "inc/Keypad.h"


void KeypadInit(void)
{
    P4->SEL0 &= ~0xFE;  // set up on P4.1-4.7
    P4->SEL1 &= ~0xFE;
    P4->DIR  &= ~0xFE;

    P4->REN |= 0x1E;
    P4->OUT |= 0x1E;
}


uint16_t ReadKeypad(uint8_t *keyValue)
{
    uint16_t row = 0;
    uint8_t i = 0;

    for(i = 0; i < 3; i++)  // iterate through columns
    {
        P4->DIR  = 0x00;
        P4->DIR |=  BIT(5 + i);
        P4->OUT &= ~BIT(5 + i);

        row = P4->IN & 0x1E;

        while ( !(P4->IN & BIT1) | !(P4->IN & BIT2) | !( P4->IN & BIT3) | !( P4->IN & BIT4) );

        if(row != 0x1E)
        {
            break;
        }
    }

    P4->DIR = 0x00;

    if(i == 3)
    {
        return 0;
    }

    switch(row)     // converts combination to keypad's number
    {
    case 0x1C:
        *keyValue = i + 1;
        break;

    case 0x1A:
        *keyValue = i + 1 + 3;
        break;

    case 0x16:
        *keyValue = i + 1 + 6;
        break;

    case 0x0E:
        *keyValue = i + 1 + 9;
        break;
    }

    if(*keyValue == 10)     // special cases
    {
        *keyValue = '*';
    }

    if(*keyValue == 11)
    {
        *keyValue = 0;
    }

    if(*keyValue == 12)
    {
        *keyValue = '#';
    }

    return 1;
}