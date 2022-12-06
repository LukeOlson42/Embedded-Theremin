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
                    // ClearMenu();
                    // execute routine
                    Theremin.State = DataInput;
                    Theremin.Menu = NextMenu;

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
    static uint8_t yearArr[4] = {0, 0, 0, 0};

    if(ReadKeypad(&Theremin.LastPressedKey))
    {
        if(Theremin.LastPressedKey == '*') // or timeout
        {
            Theremin.State = NormalOperation;
            Theremin.Menu = Main;
            Theremin.Flags.ChangeMenu = true;
            Theremin.Flags.UpdatedRTCData = true;
        }
        else if(Theremin.LastPressedKey != '#')
        {
            if(Theremin.Menu == InputYear)
            {

                yearArr[0] = yearArr[1];          // shifts numbers to left
                yearArr[1] = yearArr[2];          // shifts numbers to left
                yearArr[2] = yearArr[3];          // shifts numbers to left
                yearArr[3] = Theremin.LastPressedKey;

                char tempString[] = {
                    '0' + yearArr[0],
                    '0' + yearArr[1],
                    '0' + yearArr[2],
                    '0' + yearArr[3],
                    '\0'
                };

                DrawString(3, 9, tempString, 0xffff, 0x0000, 4);
            }
            else
            {
                inputArr[0] = inputArr[1];          // shifts numbers to left
                inputArr[1] = Theremin.LastPressedKey;

                char tempString[] = {
                    '0' + inputArr[0],
                    '0' + inputArr[1],
                    '\0'
                };

                DrawString(7, 9, tempString, 0xffff, 0x0000, 4);
            }

        }
        else // # is pressed
        {
            // uint8_t 
            if(Theremin.Menu == InputYear)
            {
                uint8_t DataToSend = yearArr[0] * 1000 + yearArr[1] * 100 + yearArr[2] * 10 + yearArr[3];
                
                WriteDataToRTC(DataToSend, Year);
            }
            else
            {
                uint8_t DataToSend = inputArr[0] * 10 + inputArr[1];

                if(Theremin.Menu == InputMonth)
                {
                    DataToSend--;
                }

                WriteDataToRTC(DataToSend, GetDesiredRTCAddress());

                // Send Data to RTC
            }

            Theremin.State = NormalOperation;
            Theremin.Flags.ChangeMenu = true;
            Theremin.Flags.UpdatedRTCData = true;
            Theremin.Menu = Main;
            // Send to RTC

            memset(inputArr, 0, sizeof(inputArr));
            memset(yearArr, 0, sizeof(yearArr));
        }
    }

    // if timeout !!!!!
}