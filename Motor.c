#include "inc/Motor.h"


void MotorInit(void)
{
    P10->SEL0 &= ~0x0F;     // pins 10.0-10.3 for motor GPIO
    P10->SEL1 &= ~0x0F;
    P10->DIR  |=  0x0F;
}


void RotateMotorRightOneStep(void)
{
    for(uint8_t i = 0; i < MOTOR_STEPS_PER_VOLUME_STEP; i++)    // four shifts per step
    {

        Theremin.MotorBits = Theremin.MotorBits >> 1;       // shift for half wave drive

        if(Theremin.MotorBits == 0b00000001)
        {
            Theremin.MotorBits = 0b00001001;
        }
        else if(Theremin.MotorBits == 0b00000100)
        {
            Theremin.MotorBits = 0b00001100;
        }
        
        P10->OUT = Theremin.MotorBits;
    }
}


void RotateMotorLeftOneStep(void)
{
    for(uint8_t i = 0; i < MOTOR_STEPS_PER_VOLUME_STEP; i++)    // four shifts per step
    {
        Theremin.MotorBits = Theremin.MotorBits << 1;       // shift for half wave drive

        if(Theremin.MotorBits == 0b00010010)
        {
            Theremin.MotorBits = 0b00000011;
        }
        else if(Theremin.MotorBits == 0b00011000)
        {
            Theremin.MotorBits = 0b00001001;
        }

        P10->OUT = Theremin.MotorBits;
    }

}