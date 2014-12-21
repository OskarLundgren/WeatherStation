#ifndef INCLUDE_KEYPAD_MODULE_H
#define INCLUDE_KEYPAD_MODULE_H
#include "Common_Variables.h"

extern int buttonWasPressed;
void Init_Keypad(void);

int Read_Keypad(void);

void Keypad_Menu_Action(int *button);

#endif