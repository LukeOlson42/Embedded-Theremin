#include "inc/Speaker.h"
#include "inc/LCD.h"
#include "inc/System.h"
#include "inc/Keypad.h"
#include "inc/MultipurposeKnob.h"
#include "inc/I2C.h"
#include "inc/SevenSegment.h"

void ThereminInit(void);

int main(void)
{

    ThereminInit();

    __enable_irq();     // enabling global interrupts

    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    for(;;)
    {
        EvaluateSystemState();
        
        UpdateVolumeBars();

        UpdateDrawnPitch();

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
    // SystemLoadPresets();

    HeartbeatTimerInit();
    TimeoutTimerInit();

    SystemWatchdogInit();

    MotorInit();
    InitLEDs();

    DrawMenuStructure();
    DrawMenuOptions(Main);

    InitSevenSegment();
}
