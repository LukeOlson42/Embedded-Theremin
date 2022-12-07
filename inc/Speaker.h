#ifndef SPEAKER_H_
#define SPEAKER_H_

#include "System.h"
#include "config.h"

typedef struct KeyChangeTable_s {
    MusicalNote Key;
    float baseFrequency;
} KeyChangeTable_s;

void SpeakerInit();
void UltrasonicInit();
void AudioSystemInit();

void OutputPitch();


#endif