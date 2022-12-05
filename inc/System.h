#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "config.h"
/**************Enumerations*********************/
typedef enum MusicalNote {
    A, Bb, B, C, Db, D, Eb, E, F, Gb, G, Ab
} MusicalNote;

typedef enum LEDAndBorderColor {
    Red, White, Green, Aqua, Blue, Violet, Yellow, NumberOfColors,
} LEDAndBorderColor;

typedef enum DayOfWeek {
    Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday
} DayOfWeek;

typedef enum MonthOfYear {
    January, February, March, April, May, June,
    July, August, September, October, November, December
} MonthOfYear;

typedef enum MenuState {
    Main, VolumeDisplay, TimeChange, PitchDisplay, NumberOfMenus, // number of menus is not last, to exclude Back and NullMenu from count
    Back, SystemStateChange, NullMenu
} MenuState;

typedef enum KnobMode {
    VolumeChange, CircleOfFifths, NumberOfModes
} KnobMode;

typedef enum SystemState {
    NormalOperation, DateTimeInput, NumberOfStates
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
        uint8_t CurrentNote; // 0 - 7, with 7 being upper octave
        // float TuningOffset;

        float SpeakerVolume;
        uint8_t DiscreteVolume;
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

        bool UpdatedRTCData;

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
extern SystemStateExecTable_s SystemStateExecTable[];

void GlobalSystemInit();

void EvaluateSystemFlags(void);
void EvaluateSystemState(void);

void SystemNormalOperation(void);
void SystemDateAndTimeInput(void);

#endif