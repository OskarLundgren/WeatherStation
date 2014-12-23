#ifndef INCLUDE_KEYPAD_MODULE_H
#define INCLUDE_KEYPAD_MODULE_H
#include "Common_Variables.h"
#include "Common_Functions.h"

extern int buttonWasPressed;
extern int nInterrupts;
extern int enteredNumber;
extern int isNegative;
extern int enteredDot;

void Init_Keypad(void);

int Read_Keypad(void);

void Keypad_Menu_Action(int *button);

#endif