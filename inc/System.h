#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>

typedef enum MusicalNote {
    A, Bb, B, C, Db, D, Eb, E, F, Gb, G, Ab
} MusicalNote;

typedef enum DayOfWeek {
    Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday
} DayOfWeek;

typedef enum MonthOfYear {
    January, February, March, April, May, June,
    July, August, September, October, November, December
} MonthOfYear;

typedef struct System {
    struct Speaker {
        MusicalNote Key;
        float SpeakerVolume;
        MusicalNote CurrentNote;
        float TuningOffset;
    } Speaker;

    float Temperature;

    struct RTC {
        struct CalendarDate{
            DayOfWeek Day;
            uint8_t   Date;
            MonthOfYear Month;
            uint16_t Year;
        } CalendarDate;

        struct Time {
            uint8_t Hour;
            uint8_t Minute;
            uint8_t Second;
        } Time;
    } RTC;

} System;

extern System Theremin;


// extern ANYTHING USED ANYWHERE

void TestFunction();


#endif