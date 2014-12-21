#include "Common_Variables.h"
#include "Common_Functions.h"
#include "Temp_Module.h"
#include "Display_Module.h"
#include <stdio.h>
#include <stdlib.h>

char converted[4];
char floatToChar[6];
float tempLog[60];
Day weekBuffer[7];
int tempLogPosition = 0;
float temp;
int oneMinute = 0;
int lastDayInBuffer = 0;
int firstDayInBuffer = -1;
Day newDay;

Day New_Day(float min, float max, float avg){
  
  Day New_Day;
    
    New_Day.minValue = min;
    New_Day.maxValue = max;
    New_Day.avgValue = avg;  
  
  return New_Day;
}


void Init_Temp(void){
  
  *AT91C_PMC_PCER = 1<<27|1<<12; //1<<27: start clock for TC0, 1<<12: start clock for PIOB
  
  *AT91C_TC0_CMR = AT91C_TC_CLKS_TIMER_DIV1_CLOCK;
  *AT91C_TC0_CCR = AT91C_TC_SWTRG|AT91C_TC_CLKEN;
  *AT91C_TC0_CMR = 3<<17; // Sets register A to load on falling edge and register B to load on rising edge
  
  NVIC_ClearPendingIRQ(TC0_IRQn);
  NVIC_SetPriority(TC0_IRQn,1);
  NVIC_EnableIRQ(TC0_IRQn);
  
  *AT91C_PIOB_PER = tempsensor;
  *AT91C_PIOB_OER = tempsensor;
  *AT91C_PIOB_PPUDR = tempsensor;
  
}


void Timer_Setup(void){
  *AT91C_PMC_PCER = 1<<30; //Enables the clock for TC3;
  
  *AT91C_TC3_CMR = AT91C_TC_CLKS_TIMER_DIV1_CLOCK|AT91C_TC_WAVESEL_UP_AUTO;
  *AT91C_TC3_CCR = AT91C_TC_SWTRG|AT91C_TC_CLKEN;
  *AT91C_TC3_RC = fastMode;
  *AT91C_TC3_IER = AT91C_TC_CPCS;
  
  NVIC_ClearPendingIRQ(TC3_IRQn);
  NVIC_SetPriority(TC3_IRQn,1);
  NVIC_EnableIRQ(TC3_IRQn);
  

}

void Temp_Measure(void){

  Setup_Interrupts(1000);
  *AT91C_PIOB_OER = tempsensor;
  *AT91C_PIOB_CODR = tempsensor;
  while(nInterrupts < 6){}
  *AT91C_PIOB_SODR = tempsensor;
  *AT91C_TC0_IER = 1<<6; // Enables interrupt when we have loaded the B register in the timer

  *AT91C_PIOB_CODR = tempsensor;
  
  delay(25);
  
  *AT91C_PIOB_SODR = tempsensor;
  *AT91C_PIOB_ODR = tempsensor;
  
  *AT91C_TC0_CCR = AT91C_TC_SWTRG; // SW reset 

}

void TC0_Handler(void){
 Setup_Interrupts(1000);
 while(nInterrupts < 400){}
  int x = *AT91C_TC0_SR;   
}

void TC3_Handler(void){
   int x = *AT91C_TC3_SR;
   oneMinute = 1;

}

void Update_Temp(void){
  temp = (*AT91C_TC0_RB-*AT91C_TC0_RA);
  temp = temp/42;  
  temp = ((temp/5.0)-273.15); 

  sprintf(floatToChar,"%f",temp);

}

void Print_Temperature(void){
  int count;
  char character;
  count = 0;
  floatToChar[4] = 0x20;
  floatToChar[5] = 0x43;
  setADP(33,1);
  while(count < 6){
   character = floatToChar[count];
   character = character - 0x20;
   Write_Data_2_Display(character);
   Write_Command_2_Display(0xC0);
   count++;
  
  }
}

void Log_Temp(void){
    tempLog[tempLogPosition++] = temp; 
}

int Size_Of_TempLog(){
  
  
  return (sizeof(tempLog)/4);

}

void Reset_TempLog(void){

	tempLogPosition = 0;

}

void Add_Values(void){
  float maxValue;
  float minValue; 
  float avgValue;
  
  maxValue = Find_Max();
  minValue = Find_Min();
  avgValue = Find_Average();
  
  
  
  newDay = New_Day(minValue,maxValue,avgValue);
  weekBuffer[lastDayInBuffer++] = newDay;
  firstDayInBuffer++;
  
}


float Find_Max(){
  
  float maxTemp;
  float thisTemp;
  maxTemp = -272;
  int i;
  for(i = 0; i < sizeof(tempLog)/4; i++){
    
    thisTemp = tempLog[i];
    if(thisTemp > maxTemp){
      maxTemp = thisTemp;
    }
  
  }
  return maxTemp;


}

float Find_Min(){
  float minTemp;
  float thisTemp;
  minTemp = 150;
  int i;
  for(i = 0; i < sizeof(tempLog)/4; i++){
    thisTemp = tempLog[i];
    if(thisTemp < minTemp){
      minTemp = thisTemp;
    
    }
  
  }
   return minTemp;
}

float Find_Average(){
  float avgTemp;
  float calculateTemp;
  calculateTemp = 0;
  int i;
  for(i = 0; i < sizeof(tempLog)/4; i++){
  
    calculateTemp = calculateTemp + tempLog[i];
  }
  
  avgTemp = calculateTemp/(sizeof(tempLog)/4);
  return avgTemp;
}


void Convert_Day_To_Char(Day* theDay, int requestedValue){
    
    
    switch(requestedValue){
    
    case 1:
      sprintf(converted,"%2.1f",theDay->minValue);
      break;
      
    case 2:
      sprintf(converted,"%2.1f",theDay->avgValue);
      break;
      
    case 3:
      sprintf(converted,"%2.1f",theDay->maxValue);
      break;
    }

}

