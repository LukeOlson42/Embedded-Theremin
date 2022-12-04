#include "inc/Speaker.h"
#include "inc/LCD.h"
#include "inc/System.h"

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

    for(;;)
    {

        // Output pitch on speaker - depends on volume, key, temperature

        // update LCD

        // update seven-segment

        // update eeprom if needed

        if(Theremin.Flags.CalculateDistance)
        {
            OutputPitch();

            Theremin.Flags.CalculateDistance = false;
        }


    }


    return 0;
}

void ThereminInit(void)
{
    LCDInit();
    AudioSystemInit();

    DrawString(0, 0, "test", 0xffff, 0x0000, 1);
    
}