#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "config.h"
#include "System.h"

void KeypadInit(void);
uint16_t ReadKeypad(uint8_t *keyValue);



#endif