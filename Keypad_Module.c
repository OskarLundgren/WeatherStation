#include "Keypad_Module.h"
#include "Common_Functions.h"
#include "Display_Module.h"
#include "Temp_Module.h"
#include "Light_Module.h"

int buttonWasPressed;
int currentMenu = 0;

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
  //*AT91C_PIOC_SODR = buffer_display_OE;
  
  
  for(col = 1<<7; col <= 512; col = col<<1){
  
    *AT91C_PIOC_SODR = columns;
    *AT91C_PIOC_CODR = col;
    theCol++;
    theRow = -1;
    
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

    switch(*button){
  
      case 1:
          {
          currentMenu = 1;
          Clear_Display();
          Print_Temperature();
          Print_Statistics();
          Print_Navigation_Buttons();
          break;
      }   
      case 2:
        {
          Find_Light();
          break;
        }
  
    }
  case 1:

    switch(*button){
      case 10:
        currentMenu = 0;
        Clear_Display();
        Print_Menu();
        Print_Temperature();
    }

  }
}


