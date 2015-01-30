#include "Keypad_Module.h"
#include "Display_Module.h"
#include "Temp_Module.h"
#include "Light_Module.h"

int buttonWasPressed;
int currentMenu = 0;
int lowerTemp;
int higherTemp;
int inputPointer = 3;
int x = 2;
int enteredNumber = 0;
int enteredDot = 0;
int isNegative = 0;
int dotPosition;
int justHigher = 0;
int justLower = 0;
int both = 0;
int Sample_Value = -1;
double digitValue = 1;
float lowerLimit = -45;
float higherLimit = 100;
float limitValue = 0;
int alarmSet = 0;
int fastModeActivated = 0;
int Timer_Interrupts;
int Graphics_Mode = 0;

void Timer_Delay_Setup(void){
  *AT91C_PMC_PCER = 1<<31; //Enables the clock for TC4;
  
  *AT91C_TC4_CMR = AT91C_TC_CLKS_TIMER_DIV1_CLOCK|AT91C_TC_WAVESEL_UP_AUTO;
  *AT91C_TC4_CCR = AT91C_TC_SWTRG;
  
  NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC4_IRQn,3);
  NVIC_EnableIRQ(TC4_IRQn);
  

}

void Start_Timer_Delay(int Interrupts_Per_Second){
     int value;
     value = (48000000/Interrupts_Per_Second)-1;
     Timer_Interrupts = 0;
     *AT91C_TC4_RC = value;
     *AT91C_TC4_CCR = AT91C_TC_SWTRG|AT91C_TC_CLKEN;
     *AT91C_TC4_IER = AT91C_TC_CPCS;

}

void Stop_Timer_Delay(void){
    *AT91C_TC4_CCR = AT91C_TC_CLKDIS;
    *AT91C_TC4_IDR = AT91C_TC_CPCS;
  
}

void TC4_Handler(void){
  int x = *AT91C_TC4_SR;
  Timer_Interrupts++;
  
}

void Init_Keypad(void){


  *AT91C_PMC_PCER = 3 << 13;
  *AT91C_PIOC_PER = databus|rows|columns;
  *AT91C_PIOC_ODR = rows;
  *AT91C_PIOC_SODR = buffer_display_OE;


  *AT91C_PIOD_PER = buffer_keypad_OE;
  *AT91C_PIOD_OER = buffer_keypad_OE;

}

int Read_Keypad(void){

  int value, col, row, theRow, theCol;
  value = 0;
  buttonWasPressed = 0;
  theRow = -1;
  theCol = -1;
  *AT91C_PIOC_OER = columns;
  *AT91C_PIOD_CODR = buffer_keypad_OE;


  for(col = 1<<7; col <= 512; col = col<<1){

    *AT91C_PIOC_SODR = columns;
    *AT91C_PIOC_CODR = col;
    theCol++;
    theRow = -1;
      Start_Timer_Delay(1000000);
      while(Timer_Interrupts < 10){}
      Stop_Timer_Delay();
    for(row = 1<<2; row <= 32; row = row<<1){    
      theRow++;
      if((*AT91C_PIOC_PDSR & row) == 0){

        value = theRow*3 + theCol + 1;
        buttonWasPressed = 1;
        break;

      }
    }  
  }

  *AT91C_PIOD_SODR = buffer_keypad_OE;
  *AT91C_PIOC_ODR = columns;

  return value;  

}

void Keypad_Menu_Action(int *button){

  switch(currentMenu){

    case 0:  
    {
      switch(*button){

        case 1:
        {
          currentMenu = 1;
          Clear_Display();
          Print_Temperature();
          Print_Statistics();
          Print_Menu_Attributes();
          while(Read_Keypad() == 1){}
          break;
        }   
        case 2:
        {
          Find_Light();
          while(Read_Keypad() == 2){}
          break;
        }
        case 3:
        {   
          currentMenu = 3;
          Clear_Display();
          Print_Temperature();
          Print_Menu_Attributes();
          while(Read_Keypad() == 3){}
            break;
        }
        
      case 4:
        {
        currentMenu = 4;
        Clear_Display();
        Print_Temperature();
        Print_Menu_Attributes();
        while(Read_Keypad() == 4){}
        break;
        }
        
      case 5:
        currentMenu = 5;
        Clear_Display();
        Print_Temperature();
        Print_Menu_Attributes();
        while(Read_Keypad() == 5){}
        break;

     
    case 10:
      
      
      if(!fastModeActivated){
        while(Read_Keypad() == 10){}
        break;
      }
        Setup_Interrupts(1000);
      while(Read_Keypad() == 10){
      
        if(nInterrupts == 700){
           *AT91C_TC3_CCR = AT91C_TC_SWTRG;
           *AT91C_TC3_RC = normalMode;
           nSample = 1440;
           currentMenu = 0;
           fastModeActivated = 0;
           
           Clear_Display();
           Setup_Interrupts(1);
           Print_Menu_Attributes();
           Print_Temperature();
           while(Read_Keypad() == 10){}
          }
        }
      
      break;
    }
    break;
    }


    case 1:
    {
      
      
      
      switch(*button){
        
        case 10:
        currentMenu = 0;
        Clear_Display();
        Print_Menu_Attributes();
        Print_Temperature();
        break;
      }
      break;
    }
    
    
  case 3:
    
    {
      switch(Read_Keypad()){
        
      case 1:
        {
        currentMenu = 31;
        justLower = 1;
        Clear_Display();
        Print_Menu_Attributes();
        Print_Temperature();
        while(Read_Keypad() == 1){}
        break;
        }
      case 2:
        {
        currentMenu = 31;
        justHigher = 1;
        Clear_Display();
        Print_Menu_Attributes();
        Print_Temperature();
        while(Read_Keypad() == 2){} 
        break;
        }
      case 3:
        {
        currentMenu = 31;
        both = 1;
        justLower = 1;
        Clear_Display();
        Print_Menu_Attributes();
        Print_Temperature();
        while(Read_Keypad() != 0){}
        break;
        }
      case 10:
        {
        currentMenu = 0;
        Clear_Display();
        Print_Menu_Attributes();
        Print_Temperature();
        break;
        }
        
      case 4:
        {
        currentMenu = 34;
        Clear_Display();
        alarmSet = 0;
        lowerLimit = -45;
        higherLimit = 100;
        Setup_Interrupts(1000);
        Print_Menu_Attributes();
        while(nInterrupts < 1000){}
        Clear_Display();
        currentMenu = 0;
        Print_Menu_Attributes();
        Print_Temperature();
        Setup_Interrupts(1);
        while(Read_Keypad() == 4){}

        break;
        
        }
      
      }
    
    
    break;
    }
    
    case 4:

        switch(*button){
        
      case 1:
        *AT91C_TC3_CCR = AT91C_TC_SWTRG;
        *AT91C_TC3_RC = fastMode;
        nSample = 1440;
        fastModeActivated = 1;
        currentMenu = 41;
        Setup_Interrupts(1000);
        Clear_Display();
        Print_Menu_Attributes();
        while(nInterrupts < 1000){}
        Clear_Display();
        currentMenu = 0;
        Print_Menu_Attributes();
        Print_Temperature();
        break;
        
        
      case 2:
        *AT91C_TC3_CCR = AT91C_TC_SWTRG;
        *AT91C_TC3_RC = fastMode;
        nSample = 60;
        fastModeActivated = 1;
        currentMenu = 41;
        Setup_Interrupts(1000);
        Clear_Display();
        Print_Menu_Attributes();
        while(nInterrupts < 1000){}
        Clear_Display();
        currentMenu = 0;
        Print_Menu_Attributes();
        Print_Temperature();
        break;
        
        
             
      
      case 10:
        currentMenu = 0;
        Clear_Display();
        
        Print_Temperature();
        Print_Menu_Attributes();
        
        while(Read_Keypad() == 10){}
     
      
      break;
        }
        break;
        
        
    case 5:
      {
        switch(Read_Keypad()){
          
        case 1:
          if(Sample_Value == 10){
            Print_To_Screen(" ",2,8);
          }
          Print_To_Screen("1",1,8);
          Sample_Value = 1;
          
          break;
          
        case 2:
          if(Sample_Value == 10){
            Print_To_Screen(" ",2,8);
          }
          Print_To_Screen("2",1,8);
          Sample_Value = 2;
          break;
          
        case 3:
          if(Sample_Value == 10){
            Print_To_Screen(" ",2,8);
          }
          Print_To_Screen("3",1,8);
          Sample_Value = 3;
          break;
          
        case 4:
          if(Sample_Value == 10){
            Print_To_Screen(" ",2,8);
          }
          Print_To_Screen("4",1,8);
          Sample_Value = 4;
          break;
          
        case 5:
         if(Sample_Value == 10){
            Print_To_Screen(" ",2,8);
          }
          Print_To_Screen("5",1,8);
          Sample_Value = 5;
          break;
          
        case 6:
          if(Sample_Value == 10){
            Print_To_Screen(" ",2,8);
          }
          Print_To_Screen("6",1,8);
          Sample_Value = 6;
          break;
          
        case 7:
          if(Sample_Value == 10){
            Print_To_Screen(" ",2,8);
          }
          Print_To_Screen("7",1,8);
          Sample_Value = 7;
          break;
          
        case 8:
          if(Sample_Value == 10){
            Print_To_Screen(" ",2,8);
          }
          Print_To_Screen("8",1,8);
          Sample_Value = 8;
          break;
          
        case 9:
          if(Sample_Value == 10){
            Print_To_Screen(" ",2,8);
          }
          Print_To_Screen("9",1,8);
          Sample_Value = 9;
          break;
          
        case 10:
          currentMenu = 0;
          Clear_Display();
          Print_Temperature();
          Print_Menu_Attributes();
          while(Read_Keypad() == 10){}
          break;
          
        case 11:
          if(Sample_Value == 1){
            Print_To_Screen("0",2,8);
            Sample_Value = 10;
          }          
          break;
         
        
        case 12:
          if(Sample_Value != -1){
            Minute_Sample_Value = Sample_Value;
            Time_To_Log = 60/Minute_Sample_Value;
            nSeconds = 0;
            
          }
          currentMenu = 51;
          Clear_Display();
          Print_Menu_Attributes();
          Setup_Interrupts(1000);
          while(nInterrupts < 1000){}
          Setup_Interrupts(1);
          currentMenu = 0;
          Clear_Display();
          Print_Menu_Attributes();
          Print_Temperature();
          while(Read_Keypad() == 12){}
          
          break;
        
        
        
        }
      Print_Menu_Attributes();
      break;
      }



    case 31:
    {

      switch(*button){

        case 11:  

        Print_To_Screen("0",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 0;
        }
        else{
          limitValue = limitValue + (0*digitValue);
          digitValue = digitValue * 0.1;
        }
        
        while(Read_Keypad() == 11){}
          break;

        case 1:
        Print_To_Screen("1",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 1;
        }
        else{
          limitValue = limitValue + (1*digitValue);
          digitValue = digitValue * 0.1;
        }
        while(Read_Keypad() == 1){}
          break;

        case 2:

        Print_To_Screen("2",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 2;
        }
        else{
          limitValue = limitValue + (2*digitValue);
          digitValue = digitValue * 0.1;
        }
        while(Read_Keypad() == 2){}
          break;

        case 3:

        Print_To_Screen("3",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 3;
        }
        else{
          limitValue = limitValue + (3*digitValue);
          digitValue = digitValue * 0.1;
        }
        while(Read_Keypad() == 3){}
          break;

        case 4:

        Print_To_Screen("4",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 4;
        }
        else{
          limitValue = limitValue + (4*digitValue);
          digitValue = digitValue * 0.1;
        }
        while(Read_Keypad() == 4){}
          break;

        case 5:

        Print_To_Screen("5",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 5;
        }
        else{
          limitValue = limitValue + (5*digitValue);
          digitValue = digitValue * 0.1;
        }
        while(Read_Keypad() == 5){}
          break;


        case 6:

        Print_To_Screen("6",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 6;
        }
        else{
          limitValue = limitValue + (6*digitValue);
          digitValue = digitValue * 0.1;
        }
        while(Read_Keypad() == 6){}
          break;     


        case 7:

        Print_To_Screen("7",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 7;
        }
        else{
          limitValue = limitValue + (7*digitValue);
          digitValue = digitValue * 0.1;
        }
        while(Read_Keypad() == 7){}
          break;


        case 8:

        Print_To_Screen("8",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 8;
        }
        else{
          limitValue = limitValue + (8*digitValue);
          digitValue = digitValue * 0.1;
        }
        while(Read_Keypad() == 8){}
          break;

        case 9:

        Print_To_Screen("9",x++,7);
        enteredNumber = 1;
        if(digitValue == 1.0){
        limitValue = (limitValue*10) + 9;
        }
        else{
          limitValue = limitValue + (9*digitValue);
          digitValue = digitValue * 0.1;
        }
        while(Read_Keypad() == 9){}
          break;

        case 10:

        if(x == 2){

          Setup_Interrupts(10);
          while(Read_Keypad() == 10){

            if(nInterrupts == 7 ){
              Print_To_Screen("-",x++,7);
              isNegative = 1;
              Print_Menu_Attributes();

              while(Read_Keypad() == 10){}
              break;
            }

          }

        }
        else{
          Print_To_Screen(" ",--x,7);
          if(x == 2){
            enteredNumber = 0;
            isNegative = 0;
            enteredDot = 0;
            Print_Menu_Attributes();
            
          }
          if(x == 3 && isNegative == 1 || x == dotPosition){
            enteredDot = 0;
            Print_Menu_Attributes();
            if(x == dotPosition){
              
              Print_To_Screen("              ",1,12);
            }
          }
          while(Read_Keypad() == 10){}
          break;
        }
        
        
      case 12:
        if(enteredNumber == 1){
          Setup_Interrupts(10);
          alarmSet = 1;
          while(Read_Keypad() == 12){
          
            if(nInterrupts == 7 && enteredDot != 1){
              dotPosition = x;
              Print_To_Screen(".",x++,7);
              enteredDot = 1;
              digitValue = 0.1;
              while(Read_Keypad() == 12){}
              
              break;
            }

            }
           if(nInterrupts < 7){
              currentMenu = 32;
              Clear_Display();
              
              if(justLower == 1){
                lowerLimit = limitValue;
                if(isNegative == 1){
                  lowerLimit = 0 - lowerLimit;
                }
                limitValue = 0;
                x = 2;
                enteredDot = 0;
                isNegative = 0;
                enteredNumber = 0;
                digitValue = 1;
                justLower = 0;
              }
              if(justHigher == 1){
                higherLimit = limitValue;
                if(isNegative == 1){
                  higherLimit = 0 - higherLimit;
                }
                limitValue = 0;
                x = 2;
                enteredDot = 0;
                isNegative = 0;
                enteredNumber = 0;
                digitValue = 1;
              }
              if(both == 1 && justHigher != 1){
                justLower = 0;
                justHigher = 1;
                currentMenu = 31;
                x = 2;
              }
              
              Print_Menu_Attributes();
              while(Read_Keypad() == 12){}
          }
          Setup_Interrupts(1);
          break;
        }
        
        
        
    }
    
    if(enteredNumber == 1){
      Print_Menu_Attributes();
    }
    if(x != 2){
      Print_To_Screen("              ",1,12);
      }
      if(x == 2 || (x==3 && isNegative == 1) || enteredDot == 1){
        Print_To_Screen("              ",25,12);
      }
    }
    
    
    case 32:
      {
        switch(Read_Keypad()){
        
        case 10:
          if(both == 1){
            justLower = 0;
            justHigher = 0;
            both = 0;
          }
          currentMenu = 31;
          Clear_Display();
          limitValue = 0;
          x = 2;
          enteredDot = 0;
          isNegative = 0;
          enteredNumber = 0;
          digitValue = 1;
          lowerLimit = 0;
          higherLimit = 0;
          Print_Menu_Attributes();
          if(alarmSet != 1){
          alarmSet = 0;
          }
          break;
          
        case 12:
          justLower = 0;
          justHigher = 0;
          both = 0;
          currentMenu = 33;
          Clear_Display();
          Print_Menu_Attributes();
          limitValue = 0;
          x = 2;
          enteredDot = 0;
          isNegative = 0;
          enteredNumber = 0;
          digitValue = 1;
          Setup_Interrupts(1000);
          while(nInterrupts < 1000){}
          Clear_Display();
          currentMenu = 0;
          
          Print_Menu_Attributes();
          Print_Temperature();
          break;
        
        }
      }
      
      
  }
  Setup_Interrupts(1);

}


