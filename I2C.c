#include "inc/I2C.h"


void InitI2C(void)
{
    EUSCI_B0->CTLW0 |= 0x01;
    EUSCI_B0->CTLW0  = 0x0F81; // 7 bit address, i2c mode, synchronous, smclk
    EUSCI_B0->BRW    = 30; // 100khz

    P1->SEL0 |= 0xC0; // pins 1.6 and 1.7
    P1->SEL1 &=~0xC0;

    EUSCI_B0->CTLW0 &=~ 0x01;
}


void I2CRead(uint8_t *data, uint8_t periphAddr, uint8_t address)
{
    InitI2C();

    EUSCI_B0->I2CSA = periphAddr;

    if(((periphAddr & 0b01111000) >> 3) == 0b00001010)
    {
        UpdateWriteProtect(true);
    }

    EUSCI_B0->CTLW0 |= 0x0010; // transmit mode
    EUSCI_B0->CTLW0 |= 0x0002; // start condition

    while(!(EUSCI_B0->IFG & 0x02)); // wait for start to finish

    EUSCI_B0->TXBUF = (uint8_t) address; // send address

    while(!(EUSCI_B0->IFG & 0x02)); // wait for txbuf to clear

    EUSCI_B0->CTLW0 &=~ 0x0010; // receive mode
    EUSCI_B0->CTLW0 |=  0x0002; // restart condition

    while(EUSCI_B0->CTLW0 & 0x02);

    EUSCI_B0->CTLW0 |= 0x0004; // set up stop condition

    while(!(EUSCI_B0->IFG & 0x01));

    *data = EUSCI_B0->RXBUF; // get data from rxbuf

    while(EUSCI_B0->CTLW0 & 0x04); // wait for stop to finish

    UpdateWriteProtect(false);
}

void I2CWrite(uint8_t *data, uint8_t periphAddr, uint8_t address)
{
    InitI2C();

    EUSCI_B0->I2CSA  = periphAddr;

    if(((periphAddr & 0b01111000) >> 3) == 0b00001010)
    {
        UpdateWriteProtect(true);
    }

    EUSCI_B0->CTLW0 |= 0x0010; // transmit mode
    EUSCI_B0->CTLW0 |= 0x0002; // start condition

    while(!(EUSCI_B0->IFG & 0x02));

    EUSCI_B0->TXBUF = (uint8_t) address;

    while(!(EUSCI_B0->IFG & 0x02));

    EUSCI_B0->TXBUF = *data; // send desired data

    while(!(EUSCI_B0->IFG & 0x02));

    EUSCI_B0->CTLW0 |= 0x0004; // send stop condition

    while(EUSCI_B0->CTLW0 & 0x04);

    UpdateWriteProtect(false);
}

void WriteDataToRTC(uint8_t data, RTC_Address address)
{
    I2CWrite(&data, RTC_PERIPH_ADD, address);
}

void ReadDataFromRTC(RTC_Data* data)
{
    uint8_t i;
    uint8_t tempData[7];

    for(i = 0; i < RTCEnd; i++) // reads all RTC registers
    {
        I2CRead(&(tempData[i]), RTC_PERIPH_ADD, i);
    }

    data->seconds = ((tempData[0] & 0x70) >> 4) * 10 + (tempData[0] & 0x0F);
    data->minute = ((tempData[1] & 0x70) >> 4) * 10 + (tempData[1] & 0x0F);
    data->hour = ((tempData[2] & 0x10) >> 4) * 10 + (tempData[2] & 0x0F);
    data->day = tempData[3] & 0x07;
    data->date = ((tempData[4] & 0x30) >> 4) * 10 + (tempData[4] & 0x0F);
    data->month = ((tempData[5] & 0x10) >> 4) * 10 + (tempData[5] & 0x0F);
    data->year = ((tempData[6] & 0xF0) >> 4) * 10 + (tempData[5] & 0x0F);
}



void UpdateWriteProtect(bool state)
{
    if(!state)
    {
        P1->OUT |= BIT5;
    }
    else
    {
        P1->OUT &= ~BIT5;
    }
}


