#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Common_Variables.h"
#include "Common_Functions.h"
#include "Display_Module.h"
#include "Keypad_Module.h"
#include "Light_Module.h"
#include "Temp_Module.h"
#include <stdlib.h>

//nu loggar den alla sju dagar pa ett fungerande satt!

int main(){
  SystemInit();
  Init_Display();
  Init_Temp();
  Timer_Setup();
  Temp_Measure();
  Update_Temp();
  Print_Temperature();
  Setup_Interrupts(1);

  
  int logTempSize;
  logTempSize = Size_Of_TempLog();
  while(1){
    
      
    if(nInterrupts == 1){
      Temp_Measure();
      Update_Temp();
      Print_Temperature();
      Setup_Interrupts(1);
    }
    if(oneMinute == 1){
    Log_Temp();
    oneMinute = 0;
    }

    if(tempLogPosition == logTempSize){
      Add_Values();
      tempLogPosition = 0;
      Reset_TempLog();
    } 
  }
  
  return 0;
}
