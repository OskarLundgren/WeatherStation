#include "Common_Functions.h"

int nInterrupts;

void Setup_Interrupts(int interrupts_per_sec){

  int Load_Value = 10500000;
  
  Load_Value = Load_Value/interrupts_per_sec;  
  Load_Value = Load_Value-1;
  
  *SYSTICK_LOAD = Load_Value;
  *SYSTICK_CTRL = 0x3;
  *SYSTICK_VAL = 0;
  
  
  nInterrupts = 0;

}

void delay(int delay){

  int i;
  
  for(i = 0; i < delay; i++){  
    asm("nop");  
  }
}

void SysTick_Handler(void){

  nInterrupts++;

}