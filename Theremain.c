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

        if(Theremin.Flags.UpdatedVolume)
        {
            UpdateVolumeBars();

            Theremin.Flags.UpdatedVolume = false;
        }
        
        if(Theremin.Flags.UpdatedPitch)
        {
            DrawBorders();

            Theremin.Flags.UpdatedPitch = false;
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
            RTC_Data data;
            ReadDataFromRTC(&data);

            GetSystemTemperature();

            Theremin.RTC.Time.Second = data.seconds;
            Theremin.RTC.Time.Minute = data.minute;
            Theremin.RTC.Time.Hour = data.hour;
            Theremin.RTC.CalendarDate.Day = (DayOfWeek) data.day;
            Theremin.RTC.CalendarDate.Date = data.date;
            Theremin.RTC.CalendarDate.Month = data.month;
            Theremin.RTC.CalendarDate.Year = data.year;

            DisplayRTCData();

            SystemKickWatchdog();

            Theremin.Flags.UpdatedRTCData = false;
        }

        if(Theremin.Flags.DebounceKnobSwitch)
        {
            if(DebounceKnobSwitch())
            {
                if(Theremin.KnobState == VolumeChange)
                {
                    Theremin.KnobState = CircleOfFifths;
                }
                else
                {
                    Theremin.KnobState = VolumeChange;
                }

                Theremin.Flags.DebounceKnobSwitch = false;
            }
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

    InitI2C();

    HeartbeatTimerInit();
    TimeoutTimerInit();

    SystemWatchdogInit();

    DrawMenuStructure();
    DrawMenuOptions(Main);
}