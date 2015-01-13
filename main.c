#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Common_Variables.h"
#include "Common_Functions.h"
#include "Display_Module.h"
#include "Keypad_Module.h"
#include "Light_Module.h"
#include "Temp_Module.h"
#include "I2C_Module.h"
#include <stdlib.h>



int main(){
  SystemInit();
  Init_Display();
  Init_Temp();
  Init_Keypad();
  Timer_Setup();
  Temp_Measure();
  Update_Temp();
  Print_Temperature();
  Light_Init();
  PWM_Setup();
  ADC_Setup();
  Setup_Interrupts(1);
  Print_Menu();
  Init_I2C();
  unsigned int testValue;
  
  
  testValue = Read_Pressure();
  
  
  int button;
  int alarmActive = 0;
  
  while(1){

    
    if(nInterrupts == 1){
      alarmActive ^= 1;
      Temp_Measure();
      Update_Temp();
      Print_Temperature();
      if(alarmSet && alarmActive%2==0 && (lowerLimit > temp || higherLimit < temp)){
        Temp_Alarm();        
      }
      
      Setup_Interrupts(1);
      
      
    }
    
    if(oneMinute == 1){
      Log_Temp();
      oneMinute = 0;
    }
    
    if(tempLogPosition == nSample){
      Add_Values();
      tempLogPosition = 0;
      
      Reset_TempLog();
      if(currentMenu == 1){
      Print_Statistics();
      }
    }
    
    button = Read_Keypad();
    if(buttonWasPressed == 1){
      buttonWasPressed = 0;
      Keypad_Menu_Action(&button);
    
    }
    

      
    }

  
  return 0;
}
