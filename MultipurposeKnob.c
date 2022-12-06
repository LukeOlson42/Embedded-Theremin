#include "inc/MultipurposeKnob.h"


void KnobInit(void)
{
    P3->SEL0 &= ~0xE0;
    P3->SEL1 &= ~0xE0;
    P3->DIR  &= ~0xE0;

    P3->REN |= BIT7;
    P3->OUT |= BIT7;

    P3->IE |= (BIT5 | BIT7);
    P3->IES |= (BIT5 | BIT7);
    P3->IFG = 0x00;

    NVIC_EnableIRQ(PORT3_IRQn);

    NVIC_SetPriority(PORT3_IRQn, 1);
}

void RightKnobTurn()
{
    if(Theremin.KnobState == VolumeChange)
    {
        Theremin.Speaker.SpeakerVolume *= 2;
        Theremin.Speaker.DiscreteVolume++;

        if(Theremin.Speaker.SpeakerVolume > 0.5)
        {
            Theremin.Speaker.SpeakerVolume = 0.5;
            Theremin.Speaker.DiscreteVolume = 10;
        }

        if(Theremin.Speaker.SpeakerVolume == 0)
        {
            Theremin.Speaker.SpeakerVolume = 0.0009765625;
            Theremin.Speaker.DiscreteVolume = 1;
        }
        else
        {
            Theremin.Flags.VolumeUp = true;
        }

    }
    else
    {

    }
}

void LeftKnobTurn()
{
    if(Theremin.KnobState == VolumeChange)
    {
        Theremin.Speaker.SpeakerVolume /= 2;
        Theremin.Speaker.DiscreteVolume--;

        if(Theremin.Speaker.SpeakerVolume < 0.0005)
        {
            Theremin.Speaker.SpeakerVolume = 0;
            Theremin.Speaker.DiscreteVolume = 0;
        }
        else
        {
            Theremin.Flags.VolumeDown = true;
        }
    }
    else
    {
        
    }
}

void PORT3_IRQHandler(void)
{
    if(P3->IFG & BIT5)
    {
        if(P3->IES & BIT5)
        {
            if(P3->IN & BIT6)
            {
                RightKnobTurn();
            }
            else
            {
                LeftKnobTurn();
            }
        }
        else
        {
            if(P3->IN & BIT6)
            {
                LeftKnobTurn();
            }
            else
            {
                RightKnobTurn();
            }
        }

        if(Theremin.KnobState == VolumeChange)
        {
            Theremin.Flags.UpdatedVolume = true;
        }

        P3->IES ^= BIT5;

        P3->IFG &= ~BIT5;
    }

    if(P3->IFG & BIT7)  // knob switch
    {
        // if pressed, enter hotswap keychange mode

        P3->IFG &= ~BIT7;
    }
}







