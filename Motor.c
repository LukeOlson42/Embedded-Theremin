#include "inc/Motor.h"


void MotorInit(void)
{
    P7->SEL0 &= ~0xf0;
    P7->SEL1 &= ~0xf0;
    P7->DIR  |=  0xf0;
}


void RotateMotorRightOneStep(void)
{
    for(uint8_t i = 0; i < MOTOR_STEPS_PER_VOLUME_STEP; i++)
    {

        Theremin.MotorBits = Theremin.MotorBits >> 1;

        if(Theremin.MotorBits == 0b00000001)
        {
            Theremin.MotorBits = 0b00001001;
        }
        else if(Theremin.MotorBits == 0b00000100)
        {
            Theremin.MotorBits = 0b00001100;
        }
        
        P7->OUT = Theremin.MotorBits << 4;
    }
}


void RotateMotorLeftOneStep(void)
{
    for(uint8_t i = 0; i < MOTOR_STEPS_PER_VOLUME_STEP; i++)
    {
        Theremin.MotorBits = Theremin.MotorBits << 1;

        if(Theremin.MotorBits == 0b00010010)
        {
            Theremin.MotorBits = 0b00000011;
        }
        else if(Theremin.MotorBits == 0b00011000)
        {
            Theremin.MotorBits = 0b00001001;
        }

        P7->OUT = Theremin.MotorBits >> 4;
    }

}