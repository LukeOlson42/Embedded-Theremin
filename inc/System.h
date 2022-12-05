#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "config.h"
/**************Enumerations*********************/
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
    Main, VolumeDisplay, TimeChange, PitchDisplay, NumberOfMenus, // number of menus is not last, to exclude Back and NullMenu from count
    Back, RoutineExec, NullMenu
} MenuState;

typedef enum KnobMode {
    VolumeChange, CircleOfFifths, NumberOfModes
} KnobMode;

typedef enum SystemState {
    NormalOperation, DateTimeInput
} SystemState;
/***********************************************/



/***************System State*******************/
typedef void (*StateExecFunc)(void);

typedef struct SystemStateExecTable_s {
    SystemState state;
    StateExecFunc funcPtr;
} SystemStateExecTable_s;

/*****************************************/


/**********System Struct**************/

typedef struct System {
    struct Speaker {
        MusicalNote Key;
        MusicalNote CurrentNote;
        // float TuningOffset;

        float SpeakerVolume;
        uint8_t DiscreteVolume;
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
        bool ChangeMenu;
        bool LargeNumberMenu;
        bool UpdatedVolume;
        bool VolumeUp;
        bool VolumeDown;

        bool UpdatedPitch;
    } Flags;

    float Temperature;

    MenuState Menu;
    MenuState PreviousMenu;
    uint8_t LastPressedKey;

    KnobMode KnobState;

    SystemState State;
} System;
/*****************************************/


extern System Theremin;

void GlobalSystemInit();
void EvaluateSystemFlags();


#endif