#include "Common_Variables.h"
#include "Common_Functions.h"

unsigned short Light_Value;

void Light_Init(void){

  *AT91C_PMC_PCER = 1<<11;
  *AT91C_PIOA_PER = 1<<4;
  *AT91C_PIOA_ODR = 1<<4;
  *AT91C_PIOA_PPUDR = 1<<4;
}

void ADC_Setup(void){

  *AT91C_PMC_PCER1 = 1<<5; // Start clock for ADC
  *ADC_MR = 1<<9;
}

void PWM_Setup(void){

  *AT91C_PIOC_PDR = 1<<24;
  *AT91C_PIOC_ABMR = 1<<24;
  *AT91C_PMC_PCER1 = 1<<4;
  *PWM_ENA = 1<<7;
  *PWM_CMR = 5;
  *PWM_CPRD = 52500;
  *PWM_CDTY = 1400;
  *PWM_CDTYUPD = 1400;
}

unsigned short Light_Measure(void){

  *ADC_CHER = 1<<2;
  *ADC_CR = 1<<1;
  Light_Value = *ADC_CDR2;
  *ADC_CR = 1<<1;
  Light_Value = *ADC_LCDR & 0xFFF;
  
  return Light_Value;
}

void Find_Light(void){

  unsigned short Minimum_Light;
  unsigned short Current_Light;
  
  Minimum_Light = 4095;
  
  unsigned int Position, Servo;
  
  Servo = 1400;
  
  *PWM_CDTYUPD = Servo;
  Setup_Interrupts(1000);
  while(nInterrupts < 400){}
  
  for(Servo = 1400; Servo < 7000; Servo = Servo + 100){
  
    Current_Light = Light_Measure();
  
    if(Current_Light < Minimum_Light){
    
      Minimum_Light = Current_Light;
      Position = Servo;      
    }
    
    Setup_Interrupts(1000);
    
    while(nInterrupts < 10){}
    
    *PWM_CDTYUPD = Servo;
    
  }

  Setup_Interrupts(1000);
  
  while(nInterrupts < 100){}
  
  *PWM_CDTYUPD = Position;  
  
  
  Setup_Interrupts(1);
  
}
