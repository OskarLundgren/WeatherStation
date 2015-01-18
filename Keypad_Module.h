#ifndef INCLUDE_KEYPAD_MODULE_H
#define INCLUDE_KEYPAD_MODULE_H
#include "Common_Variables.h"
#include "Common_Functions.h"

extern int buttonWasPressed;
extern int nInterrupts;
extern int enteredNumber;
extern int isNegative;
extern int enteredDot;
extern int justLower;
extern int justHigher;
extern int both;
extern float lowerLimit;
extern float higherLimit;
extern int alarmSet;
extern int fastModeActivated;
extern int Sample_Value;

void Timer_Delay_Setup(void);

void Start_Timer_Delay(int Interrupts_Per_Second);

void Stop_Timer_Delay(void);

void Init_Keypad(void);

int Read_Keypad(void);

void Keypad_Menu_Action(int *button);

#endif