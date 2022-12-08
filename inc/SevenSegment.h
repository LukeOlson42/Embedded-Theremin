#ifndef SEVENSEGMENT_H_
#define SEVENSEGMENT_H_

#include "config.h"
#include "System.h"

typedef enum SevenSegmentCommand 
{
    NoOp = 0x00,
    Digit0 = 0x01,
    Digit1 = 0x02,
    Digit2 = 0x03,
    Digit3 = 0x04,
    Digit4 = 0x05,
    Digit5 = 0x06,
    Digit6 = 0x07,
    Digit7 = 0x08,
    DecodeMode = 0x09,
    Intensity = 0x0A,
    ScanLimit = 0x0B,
    Shutdown = 0x0C,
    DisplayTest = 0x0F,
} SevenSegmentCommand;


static uint8_t queuedData;

void InitSevenSegment(void);
void InitSevenSegmentSPI(void);
void InitSevenSegmentPins(void);
void InitSevenSegmentChip(void);

void SendSevenSegmentMessage(SevenSegmentCommand command, uint8_t data);



#endif