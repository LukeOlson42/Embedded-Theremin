#include "inc/Speaker.h"
#include "inc/LCD.h"
#include "inc/System.h"
#include "inc/Keypad.h"
#include "inc/MultipurposeKnob.h"

/**888************************
 * 
 * IF LCD IS MESSED UP SWITCH BRW IN .C FILE TO 4, AND CHANGE SMCLK BACK TO 0X20000000
 * 
 * 
*/

void ThereminInit(void);

int main(void)
{

    ThereminInit();

    Theremin.Flags.CalculateDistance = false;

    __enable_irq();     // enabling global interrupts

    P1->DIR |= BIT0;

    for(;;)
    {

        // Output pitch on speaker - depends on volume, key, temperature

        // update LCD

        // update seven-segment

        // update eeprom if needed

        if(ReadKeypad(&Theremin.LastPressedKey))        // put in function pointer table depending on system state
        {
            if(Theremin.LastPressedKey < MAX_MENU_OPTIONS)
            {
                MenuState NextMenu = FindNextMenu(Theremin.Menu, Theremin.LastPressedKey);
                if(NextMenu < NumberOfMenus)
                {
                    DrawMenuOptions(NextMenu);
                }
            }
        }

        if(Theremin.Flags.CalculateDistance)            // put in flag eval routine
        {
            OutputPitch();

            Theremin.Flags.CalculateDistance = false;
        }

    }


    return 0;
}

void ThereminInit(void)
{
    GlobalSystemInit();

    LCDInit();
    AudioSystemInit();

    KeypadInit();
    KnobInit();

    DrawMenuStructure();
    DrawMenuOptions(Main);
}