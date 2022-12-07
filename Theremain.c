#include "inc/Speaker.h"
#include "inc/LCD.h"
#include "inc/System.h"
#include "inc/Keypad.h"
#include "inc/MultipurposeKnob.h"
#include "inc/I2C.h"

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

    __enable_irq();     // enabling global interrupts

    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    for(;;)
    {

        // Output pitch on speaker - depends on volume, key, temperature

        // update LCD

        // update seven-segment

        // update eeprom if needed

        EvaluateSystemState();
        
        UpdateVolumeBars();

        EvaluateSystemFlags();

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

    InitI2C();

    HeartbeatTimerInit();
    TimeoutTimerInit();

    SystemWatchdogInit();

    MotorInit();
    InitLEDs();

    DrawMenuStructure();
    DrawMenuOptions(Main);

    // SystemLoadPresets();
}
