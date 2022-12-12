#ifndef I2C_H_
#define I2C_H_

#include "config.h"

typedef enum RTC_Address { // names of RTC registers' internal addresses
    Seconds = 0x00,
    Minutes = 0x01,
    Hours,
    Day,
    Date,
    Month,
    Year,
    RTCEnd,
} RTC_Address;

typedef struct RTC_Data {
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint16_t year;
    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;
} RTC_Data;

void InitI2C(void);
void I2CRead(uint8_t *data, uint8_t periphAddr, uint8_t address);
void I2CWrite(uint8_t *data, uint8_t periphAddr, uint8_t address);

void ReadDataFromRTC(RTC_Data* data);
void WriteDataToRTC(uint8_t data, RTC_Address address);


#endif