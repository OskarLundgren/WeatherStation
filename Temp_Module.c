#include "Common_Variables.h"
#include "Common_Functions.h"
#include "Temp_Module.h"
#include "Display_Module.h"
#include "Memory_Module.h"
#include <stdio.h>
#include <stdlib.h>

char converted[4];
char floatToChar[6];
float tempLog[1440];
Day weekBuffer[7];
int tempLogPosition = 0;
float temp;
int nSeconds = 0;
int lastDayInBuffer = 0;
int firstDayInBuffer = -1;
int nSample = 1440;
int Minute_Sample_Value = 1;
Day newDay;
int Time_To_Log = 60;
float minuteLog[10];
int Minute_Log_Position = 0;
int Time_To_Add_Value = 0;

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
  NVIC_SetPriority(TC0_IRQn,2);
  NVIC_EnableIRQ(TC0_IRQn);
  
  *AT91C_PIOB_PER = tempsensor;
  *AT91C_PIOB_OER = tempsensor;
  *AT91C_PIOB_PPUDR = tempsensor;
  
}

void Timer_Setup(void){
  *AT91C_PMC_PCER = 1<<30; //Enables the clock for TC3;
  
  *AT91C_TC3_CMR = AT91C_TC_CLKS_TIMER_DIV1_CLOCK|AT91C_TC_WAVESEL_UP_AUTO;
  *AT91C_TC3_CCR = AT91C_TC_SWTRG|AT91C_TC_CLKEN;
  *AT91C_TC3_RC = normalMode;
  *AT91C_TC3_IER = AT91C_TC_CPCS;
  
  NVIC_ClearPendingIRQ(TC3_IRQn);
  NVIC_SetPriority(TC3_IRQn,0);
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
 Setup_Interrupts(1);
}

void TC3_Handler(void){
   int x = *AT91C_TC3_SR;
   nSeconds++;
   if(nSeconds == Time_To_Log){
      Log_Temp();
      nSeconds = 0;
      Time_To_Add_Value = 1;
   }

}

void Update_Temp(void){
  temp = (*AT91C_TC0_RB-*AT91C_TC0_RA);
  temp = temp/42;  
  temp = ((temp/5.0)-273.15); 

  sprintf(floatToChar,"%f",temp);

}

void Temp_Alarm(void){
  int addressPointer = 0;
  setADP(addressPointer,1);
  
  while(addressPointer < 40){
  
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xC0);
    addressPointer++;
  }
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
    minuteLog[Minute_Log_Position++] = temp;
    if(Minute_Log_Position == Minute_Sample_Value){
      
      Log_Minute_Average(minuteLog,Minute_Sample_Value);
      Minute_Log_Position = 0;
    
    }
}

int Size_Of_TempLog(){
  
  
  return (sizeof(tempLog)/4);

}

void Reset_TempLog(void){

	tempLogPosition = 0;

}

void Log_Minute_Average(float Log[], int Array_Length){
  float avgValue; 
  avgValue = Find_Average(Log,Array_Length);
  tempLog[tempLogPosition++] = avgValue;
  

}

void Add_Values(void){
  float maxValue;
  float minValue; 
  float avgValue;
  
  maxValue = Find_Max();
  minValue = Find_Min();
  avgValue = Find_Average(tempLog,nSample);
  
  
  
  newDay = New_Day(minValue,maxValue,avgValue);
  weekBuffer[lastDayInBuffer++] = newDay;
  //Add_Day_To_Memory(newDay);
  if(lastDayInBuffer == 7){
    lastDayInBuffer = 0;
  }
}


float Find_Max(){
  
  float maxTemp;
  float thisTemp;
  maxTemp = -272;
  int i;
  for(i = 0; i < nSample; i++){
    
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
  for(i = 0; i < nSample; i++){
    thisTemp = tempLog[i];
    if(thisTemp < minTemp){
      minTemp = thisTemp;
    
    }
  
  }
   return minTemp;
}

float Find_Average(float Log[], int Array_Length){
  float avgTemp;
  float calculateTemp;
  calculateTemp = 0;
  int i;
  for(i = 0; i < Array_Length; i++){
  
    calculateTemp = calculateTemp + Log[i];
  }
  
  avgTemp = calculateTemp/Array_Length;
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

