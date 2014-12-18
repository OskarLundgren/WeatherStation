#include "Keypad_Module.h"
#include "Common_Functions.h"


void Init_Keypad(void){
  
  *AT91C_PMC_PCER = 3 << 13;
  *AT91C_PIOD_PER = databus|buffer_keypad_OE|rows|columns;
  *AT91C_PIOD_OER = buffer_keypad_OE|columns;
  *AT91C_PIOC_ODR = databus|rows;
  *AT91C_PIOD_SODR = buffer_keypad_OE;
}

int Read_Keypad(void){
  
  int value, col, row, theRow, theCol;
  theRow = -1;
  theCol = -1;
  *AT91C_PIOC_OER = columns;
  *AT91C_PIOD_CODR = buffer_keypad_OE;
  
  for(col = 1<<7; col <= 512; col = col<<1){
  
    *AT91C_PIOC_SODR = columns;
    *AT91C_PIOC_CODR = col;
    theCol++;
    theRow = -1;
    
    for(row = 1<<2; row <= 32; row = row<<1){    
      theRow++;
      if((*AT91C_PIOC_PDSR & row) == 0){
      
      	value = theRow*3 + theCol + 1;
      	break;
      }    
    }  
  }
  
  *AT91C_PIOD_SODR = buffer_keypad_OE;
  *AT91C_PIOC_ODR = columns;
  
  return value;  

}