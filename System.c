#include "inc/System.h"
#include "inc/LCD.h"
#include "inc/Keypad.h"

/******Global System Variable*******/
System Theremin;
/***********************************/


SystemStateExecTable_s SystemStateExecTable[NumberOfStates] = {
    {NormalOperation,  SystemNormalOperation},
    {  DateTimeInput, SystemDateAndTimeInput}
};



void GlobalSystemInit()
{
    memset(&Theremin, 0, sizeof(Theremin));

    Theremin.Speaker.SpeakerVolume = 0.015625;
    Theremin.Speaker.DiscreteVolume = 5;
    Theremin.Flags.CalculateDistance = false;

    Theremin.Flags.ChangeMenu = false;
    Theremin.Flags.LargeNumberMenu = false;
    Theremin.Flags.UpdatedPitch = false;
    Theremin.Flags.UpdatedVolume = false;
    Theremin.Flags.UpdatedRTCData = true;
    Theremin.Flags.VolumeUp = false;
    Theremin.Flags.VolumeDown = false;
    
    Theremin.KnobState = VolumeChange;
    Theremin.State = NormalOperation;
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
                if(NextMenu == Back)
                {
                    Theremin.Menu = Theremin.PreviousMenu;
                    Theremin.Flags.LargeNumberMenu = false;
                    Theremin.Flags.UpdatedPitch = false;
                    Theremin.Flags.UpdatedVolume = false;
                }
                else if(NextMenu == SystemStateChange)
                {
                    // execute routine
                }
                else
                {
                    if(NextMenu == VolumeDisplay)
                    {
                        Theremin.Flags.LargeNumberMenu = true;
                        Theremin.Flags.UpdatedVolume = true;
                    }
                    else if(NextMenu == PitchDisplay)
                    {
                        Theremin.Flags.LargeNumberMenu = true;
                        Theremin.Flags.UpdatedPitch = true;
                    }
                    else
                    {
                        Theremin.Flags.LargeNumberMenu = false;
                    }

                    Theremin.PreviousMenu = Theremin.Menu;
                    Theremin.Menu = NextMenu;
                }

                Theremin.Flags.ChangeMenu = true;
            }
        }
    }
}


void SystemDateAndTimeInput(void)
{

}