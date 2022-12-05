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

        EvaluateSystemState();
        
        UpdateVolumeBars();

        if(Theremin.Flags.UpdatedVolume)
        {
            UpdateVolumeBars();

            Theremin.Flags.UpdatedVolume = false;
        }

        if(Theremin.Flags.CalculateDistance)            // put in flag eval routine
        {
            OutputPitch();

            Theremin.Flags.CalculateDistance = false;
        }

        if(Theremin.Flags.ChangeMenu)
        {
            ClearMenu();
            DrawMenuOptions(Theremin.Menu);
            if(Theremin.Menu == VolumeDisplay)
            {
                DisplayVolumeBars();
            }

            Theremin.Flags.ChangeMenu = false;
        }

        if(Theremin.Flags.UpdatedRTCData)
        {
            DisplayRTCData();

            Theremin.Flags.UpdatedRTCData = false;
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