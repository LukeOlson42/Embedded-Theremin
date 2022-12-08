#include "inc/Speaker.h"
#include "inc/LCD.h"
#include "inc/System.h"
#include "inc/Keypad.h"
#include "inc/MultipurposeKnob.h"
#include "inc/I2C.h"
#include "inc/SevenSegment.h"


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

    // P7->OUT &= ~BIT2;

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

    // InitSevenSegment();

    // SendSevenSegmentMessage(Digit5, 0x09);
    // SendSevenSegmentMessage(Digit4, 0x08);
    // SendSevenSegmentMessage(Digit3, 0x07);

    // SystemLoadPresets();
    // P7->OUT |= BIT2;
}
