#ifndef INCLUDE_COMMON_FUNCTIONS_H
#define INCLUDE_COMMON_FUNCTIONS_H

#include "system_sam3x.h"
#include "at91sam3x8.h"

#define SYSTICK_CTRL ((volatile unsigned int*)0xE000E010)
#define SYSTICK_LOAD ((volatile unsigned int*)0xE000E014)
#define SYSTICK_VAL ((volatile unsigned int*)0xE000E018)

extern int nInterrupts;

void Setup_Interrupts(int interrupts_per_sec);

void delay(int delay);

#endif