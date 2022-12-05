#include "inc/System.h"

/******Global System Variable*******/
System Theremin;
/***********************************/






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
    Theremin.KnobState = VolumeChange;
}


void EvaluateSystemFlags(void)
{

}