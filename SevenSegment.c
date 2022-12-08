#include "inc/SevenSegment.h"

void InitSevenSegment(void)
{
    InitSevenSegmentSPI();
    InitSevenSegmentPins();
    InitSevenSegmentChip();
}

void InitSevenSegmentSPI(void)
{
    EUSCI_B1->CTLW0 |= 0x0001;
    EUSCI_B1->CTLW0 |= 0x29C0;      
    EUSCI_B1->BRW = 1;             // 300khz
    EUSCI_B1->CTLW0 &=~ 0x0001;
}

void InitSevenSegmentPins(void)
{
    P6->SEL0 |=  0x18;              // 6.3 - clk, 6.4 - MOSI, 6.5 - MISO
    P6->SEL1 &= ~0x18;

    P7->SEL0 &= ~0x04;
    P7->SEL1 &= ~0x04;
    P7->DIR  |= 0x04;
    P7->OUT  |= 0x04;
}

void InitSevenSegmentChip(void)
{
    SendSevenSegmentMessage(DecodeMode, 0xFF);
    SendSevenSegmentMessage(Intensity, 0x02);
    SendSevenSegmentMessage(ScanLimit, 0x07);
    SendSevenSegmentMessage(Shutdown, 0x01);
    // SendSevenSegmentMessage(DisplayTest, 0x01);
    SendSevenSegmentMessage(Digit0, 0x00);
    SendSevenSegmentMessage(Digit1, 0x00);
    SendSevenSegmentMessage(Digit2, 0x00);
    SendSevenSegmentMessage(Digit3, 0x00);
    SendSevenSegmentMessage(Digit4, 0x00);
    SendSevenSegmentMessage(Digit5, 0x00);
    SendSevenSegmentMessage(Digit6, 0x00);
    SendSevenSegmentMessage(Digit7, 0x00);
}

void SendSevenSegmentMessage(SevenSegmentCommand command, uint8_t data)
{
    while(!(EUSCI_B1->IFG & BIT1));

    P7->OUT &= ~0x04;

    EUSCI_B1->TXBUF = (uint8_t) command;
    while(!(EUSCI_B1->IFG & BIT1));
    EUSCI_B1->TXBUF = data;
    while(!(EUSCI_B1->IFG & BIT1));
    while(!(EUSCI_B1->IFG & BIT1));

    SystemSysTickDelay();

    P7->OUT |= BIT2;

    // P1->OUT &= ~BIT0;
}
