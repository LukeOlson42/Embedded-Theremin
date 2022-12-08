#include "inc/System.h"
#include "inc/I2C.h"
#include "inc/LCD.h"
#include "inc/Keypad.h"
#include "inc/Speaker.h"
#include "inc/MultipurposeKnob.h"
#include "inc/SevenSegment.h"

/******Global System Variable*******/
System Theremin;
/***********************************/


SystemStateExecTable_s SystemStateExecTable[NumberOfStates] = {
    {NormalOperation,  SystemNormalOperation},
    {  DataInput, SystemDataInput},
};



void GlobalSystemInit()
{
    memset(&Theremin, 0, sizeof(Theremin));

    Theremin.Speaker.SpeakerVolume = 0.015625;
    Theremin.Speaker.DiscreteVolume = 5;
    Theremin.Flags.CalculateDistance = false;

    Theremin.Flags.ChangeMenu = false;
    Theremin.Flags.UpdatedPitch = false;
    Theremin.Flags.UpdatedVolume = false;
    Theremin.Flags.UpdatedRTCData = true;
    Theremin.Flags.VolumeUp = false;
    Theremin.Flags.VolumeDown = false;
    Theremin.Flags.DebounceKnobSwitch = false;
    
    Theremin.KnobState = VolumeChange;
    Theremin.State = NormalOperation;

    Theremin.Speaker.Key = Eb;

    Theremin.RTC.Time.Hour = 7;
    Theremin.RTC.Time.Minute = 34;
    Theremin.RTC.Time.Second = 22;

    Theremin.MotorBits = 0b00001100;
}


void EvaluateSystemFlags(void)
{
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
        // P7->OUT &= ~BIT2;
        RTC_Data data;
        ReadDataFromRTC(&data);

        GetSystemTemperature();

        // SendSevenSegmentMessage(Digit1, (uint8_t) Theremin.Temperature / 10);
        // SendSevenSegmentMessage(Digit2, (uint8_t) Theremin.Temperature % 10);

        Theremin.RTC.Time.Second = data.seconds;
        Theremin.RTC.Time.Minute = data.minute;
        Theremin.RTC.Time.Hour = data.hour;
        Theremin.RTC.CalendarDate.Day = (DayOfWeek) data.day;
        Theremin.RTC.CalendarDate.Date = data.date;
        Theremin.RTC.CalendarDate.Month = data.month;
        Theremin.RTC.CalendarDate.Year = data.year;

        DisplayRTCData();

        // SystemSavePresets();

        SystemKickWatchdog();

        // P7->OUT |= BIT2;
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


void EvaluateSystemState(void)
{
    for(uint8_t i = 0; i < NumberOfStates; i++)
    {
        if(Theremin.State == SystemStateExecTable[i].state)
        {
            if(SystemStateExecTable[i].funcPtr != NULL)
            {
                (*SystemStateExecTable[i].funcPtr)();
            }
        }
    }
}


void SystemNormalOperation(void)
{
    if(ReadKeypad(&Theremin.LastPressedKey))        // put in function pointer table depending on system state, this is NormalOperation
    {
        if(Theremin.LastPressedKey <= MAX_MENU_OPTIONS)
        {
            MenuState NextMenu = FindNextMenu(Theremin.Menu, Theremin.LastPressedKey);
            if(NextMenu != NullMenu)
            {
                if(NextMenu >= InputDay && NextMenu < NumberOfMenus)
                {
                    // execute routine
                    Theremin.State = DataInput;
                    Theremin.Menu = NextMenu;

                    EnableTimeoutTimer();

                }
                else
                {
                    if(NextMenu == VolumeDisplay)
                    {
                        Theremin.Flags.UpdatedVolume = true;
                    }
                    else if(NextMenu == PitchDisplay)
                    {
                        Theremin.Flags.UpdatedPitch = true;
                    }
                    else
                    {

                    }

                    Theremin.Menu = NextMenu;
                }

                Theremin.Flags.ChangeMenu = true;
            }
        }
    }
}


void SystemDataInput(void)
{
    static uint8_t inputArr[2] = {0, 0};

    if(Theremin.Flags.SystemTimeout)
    {
        Theremin.Menu = Main;
        Theremin.Flags.ChangeMenu = true;
        Theremin.State = NormalOperation;

        Theremin.Flags.SystemTimeout = false;
    }

    if(ReadKeypad(&Theremin.LastPressedKey))
    {
        if(Theremin.LastPressedKey == '*') // or timeout
        {
            Theremin.State = NormalOperation;
            Theremin.Menu = Main;
            Theremin.Flags.ChangeMenu = true;
            Theremin.Flags.UpdatedRTCData = true;

            DisableTimeoutTimer();
        }
        else if(Theremin.LastPressedKey != '#')
        {
            ResetTimeoutTimerCount();

            inputArr[0] = inputArr[1];          // shifts numbers to left
            inputArr[1] = Theremin.LastPressedKey;

            char tempString[] = {
                '0' + inputArr[0],
                '0' + inputArr[1],
                '\0'
            };

            DrawString(7, 9, tempString, 0xffff, 0x0000, 4);

        }
        else // # is pressed
        {
            uint8_t DataToSend = (inputArr[0] << 4) | inputArr[1];

            if(Theremin.Menu == InputMonth)
            {
                DataToSend--;
            }


            WriteDataToRTC(DataToSend, GetDesiredRTCAddress());

            Theremin.State = NormalOperation;
            Theremin.Flags.ChangeMenu = true;
            Theremin.Flags.UpdatedRTCData = true;
            Theremin.Menu = Main;

            DisableTimeoutTimer();

            memset(inputArr, 0, sizeof(inputArr));
        }
    }

}



void HeartbeatTimerInit(void)
{
    TIMER32_1->LOAD = 48000000; // one second
    TIMER32_1->CONTROL = 0xE2; // interrupts, periodic, wrapping

    NVIC_EnableIRQ(T32_INT1_IRQn);
    NVIC_SetPriority(T32_INT1_IRQn, 1);
}


void TimeoutTimerInit(void)
{
    TIMER32_2->LOAD = (uint32_t)((uint32_t) 48000000 * (uint32_t) 60); // one minute
    TIMER32_2->CONTROL = 0x62;

    NVIC_EnableIRQ(T32_INT2_IRQn);
    NVIC_SetPriority(T32_INT2_IRQn, 2);
}

void EnableTimeoutTimer(void)
{
    TIMER32_2->CONTROL |= BIT7;
}

void DisableTimeoutTimer(void)
{
    TIMER32_2->CONTROL &= ~BIT7;
}

void ResetTimeoutTimerCount(void)
{
    DisableTimeoutTimer();
    EnableTimeoutTimer();
}

void GetSystemTemperature(void)
{
    uint8_t tempByte1;
    uint8_t tempByte2;

    I2CRead(&tempByte1, RTC_PERIPH_ADD, (RTC_Address) 0x11);
    // I2CRead(&tempByte2, RTC_PERIPH_ADD, (RTC_Address) 0x12);

    Theremin.Temperature = tempByte1; //(tempByte1 << 2) | (tempByte2 >> 6);
}

void SystemWatchdogInit(void)
{
    WDT_A->CTL = 0x5A00 | 0x2B; // unlocks, set to ACLK, watchdog mode, clears counter, /2^19 divider

    P1->SEL0 &=~ BIT4;
    P1->SEL1 &=~ BIT4;
    P1->DIR  &=~ BIT4;
    P1->REN  |=  BIT4;
    P1->OUT  |=  BIT4;
    P1->IES  |=  BIT4;
    P1->IE   |=  BIT4;
    P1->IFG = 0;

    NVIC_EnableIRQ(PORT1_IRQn);
}


void SystemKickWatchdog(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | BIT3 | 0x2B; // reset watchdog timer
}


void SystemSavePresets(void)
{
    I2CWrite((uint8_t*) &Theremin.Speaker.Key, EEPROM_ADDR, 0x00);
    I2CWrite(&Theremin.Speaker.DiscreteVolume, EEPROM_ADDR, 0x01);
}


void SystemLoadPresets(void)
{
    I2CRead((uint8_t*) &Theremin.Speaker.Key, EEPROM_ADDR, 0x00);
    I2CRead(&Theremin.Speaker.DiscreteVolume, EEPROM_ADDR, 0x01);
}


void PORT1_IRQHandler(void)
{
    if(P1->IFG & BIT4)
    {
        WDT_A->CTL |= 0x1100;

        P1->IFG = 0;
    }
}


void T32_INT1_IRQHandler(void)
{
    Theremin.Flags.UpdatedRTCData = true;

    TIMER32_1->INTCLR = 1;
}

void T32_INT2_IRQHandler(void)
{
    Theremin.Flags.SystemTimeout = true;

    TIMER32_2->INTCLR = 1;
}
