#include "inc/Speaker.h"

/********Key Table********/
static const KeyChangeTable_s KeyChangeTable[] = {
    {A,  220.00}, {Bb, 233.08}, {B,  246.92}, {C,  261.63},
    {Db, 277.18}, {D,  293.66}, {Eb, 311.13}, {E,  329.63},
    {F,  349.23}, {Gb, 369.99}, {G,  392.00}, {Ab, 415.30},
};
/*************************/

void SpeakerInit()
{
    TIMER_A2->CTL = 0x0214;
    TIMER_A2->CCR[0] = 10 * 3000;
    TIMER_A2->CCR[1] = 1;
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

    // init p5.6 as LED output
    P5->SEL0 |=  BIT6;
    P5->SEL1 &= ~BIT6;
    P5->DIR  |=  BIT6;
}

void UltrasonicInit()
{
    TIMER_A0->CTL = 0x0294; // SMCLK, /4 divider, up mode, no interrupts
    TIMER_A0->CCR[0] = 4000 * 4;//2792 * 4;
    TIMER_A0->CCR[1] = 8;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCTL[2] = 0xC910;

    // init p2.4 as trigger output
    P2->SEL0 |=  BIT4;
    P2->SEL1 &= ~BIT4;
    P2->DIR  |=  BIT4;

    // init p2.5 as echo input
    P2->SEL0 |=  BIT5;
    P2->SEL1 &= ~BIT5;
    P2->DIR  &= ~BIT5;

}

void AudioSystemInit()
{
    SpeakerInit();
    UltrasonicInit();
    NVIC_EnableIRQ(TA0_N_IRQn);
    NVIC_SetPriority(TA0_N_IRQn, 4);
}

static float SpeakerGetKeyBasePitch(void)
{
    for(uint8_t i = 0; i < NumberOfKeys; i++)
    {
        if(KeyChangeTable[i].Key == Theremin.Speaker.Key)
        {
            return KeyChangeTable[i].baseFrequency;
        }
    }

    return 220.f; // return default frequency of A
}

void OutputPitch()
{
    static uint8_t previousPitch = 0;

    float baseFrequency = SpeakerGetKeyBasePitch();

    static const uint8_t stepData[8] = {0, 2, 4, 5, 7, 9, 11, 12};
    static const float TwelfthRootOfTwo = 1.05946;
    float NoteDifference = TwelfthRootOfTwo;

    if(previousPitch != Theremin.Speaker.SensorDistanceInches)
    {
        Theremin.Flags.UpdatedPitch = true;
    }

    if(Theremin.Speaker.SensorDistanceInches >= 3 && Theremin.Speaker.SensorDistanceInches <= 10)
    {
        previousPitch = Theremin.Speaker.SensorDistanceInches;
        Theremin.Speaker.CurrentNote = Theremin.Speaker.SensorDistanceInches - 3;

        if(Theremin.Speaker.CurrentNote == 0)
        {
            NoteDifference = 1;
        }
        else
        {
            for(uint8_t i = 1; i < stepData[Theremin.Speaker.CurrentNote]; i++)
            {
                NoteDifference *= TwelfthRootOfTwo;
            }
        }

        TIMER_A2->CCR[0] = 3000000 * 1.f / (baseFrequency * NoteDifference);
    }
    else
    {
        previousPitch = 0;
        TIMER_A2->CCR[0] = 0;
    }

    TIMER_A2->CCR[1] = TIMER_A2->CCR[0] * Theremin.Speaker.SpeakerVolume;
}




void TA0_N_IRQHandler(void)
{
    static bool risingEdge = true;

    static uint16_t echoPulseREdge = 0;
    static uint16_t echoPulseFEdge = 0;
    static uint16_t trigToEchoPeriod = 0;

    if(TIMER_A0->CCTL[2] & BIT0)
    {
        if(!Theremin.Flags.CalculateDistance)   // if not ready to calculate distance (i.e. missing falling edge)
        {
            if(risingEdge)
            {
                echoPulseREdge = TIMER_A0->CCR[2];
                risingEdge = false;
            }
            else
            {
                echoPulseFEdge = TIMER_A0->CCR[2];

                trigToEchoPeriod = echoPulseFEdge - echoPulseREdge;

                Theremin.Speaker.SensorDistanceInches = trigToEchoPeriod * PERIOD_TO_INCHES_RATIO;

                Theremin.Flags.CalculateDistance = true;
                risingEdge = true;
            }
        }

        TIMER_A0->CCTL[2] &= ~BIT1;
        TIMER_A0->CCTL[2] &= ~BIT0;
    }
}
