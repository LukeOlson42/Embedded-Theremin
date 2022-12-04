#include "inc/System.h"

System Theremin;

void GlobalSystemInit()
{
    memset(&Theremin, 0, sizeof(Theremin));
    Theremin.Speaker.SpeakerVolume = 0.5;
}
