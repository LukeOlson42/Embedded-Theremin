#ifndef MULTIPURPOSEKNOB_H_
#define MULTIPURPOSEKNOB_H_

#include "config.h"
#include "System.h"
#include "Motor.h"

void KnobInit(void);

void RightKnobTurn(void);
void LeftKnobTurn(void);

bool DebounceKnobSwitch(void);


#endif