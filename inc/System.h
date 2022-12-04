#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "config.h"

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

typedef enum MenuState {
    Main, DateChange, TimeChange, KeyChange, NumberOfMenus, // number of menus is not last, to exclude Back and NullMenu from count
    Back, NullMenu
} MenuState;

typedef enum KnobMode {
    VolumeChange, CircleOfFifths, NumberOfModes
} KnobMode;

typedef struct System {
    struct Speaker {
        MusicalNote Key;
        MusicalNote CurrentNote;
        // float TuningOffset;

        float SpeakerVolume;
        float SensorPeriod;
        uint8_t SensorDistanceInches;
    } Speaker;

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

    struct Flags {
        bool CalculateDistance;
    } Flags;

    float Temperature;
    MenuState Menu;
    uint8_t LastPressedKey;

    KnobMode KnobState;
} System;

extern System Theremin;

void GlobalSystemInit();


#endif