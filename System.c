#include "inc/System.h"
#include "inc/I2C.h"
#include "inc/LCD.h"
#include "inc/Keypad.h"

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
    
    Theremin.KnobState = VolumeChange;
    Theremin.State = NormalOperation;

    Theremin.RTC.Time.Hour = 7;
    Theremin.RTC.Time.Minute = 34;
    Theremin.RTC.Time.Second = 22;
}


void EvaluateSystemFlags(void)
{

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


void T32_INT1_IRQHandler(void)
{
    Theremin.Flags.UpdatedRTCData = true;

    TIMER32_1->INTCLR = 1;
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


void T32_INT2_IRQHandler(void)
{
    Theremin.Flags.SystemTimeout = true;

    TIMER32_2->INTCLR = 1;
}
