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

    for(row = 1<<2; row <= 32; row = row<<1){    
      theRow++;
      if((*AT91C_PIOC_PDSR & row) == 0){

        value = theRow*3 + theCol + 1;
        buttonWasPressed = 1;
        break;
      }
      delay(50);
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
          break;
        }   
        case 2:
        {
          Find_Light();
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

      }
      break;
    }


    case 1:
    {
      switch(*button){

        case 10:
        currentMenu = 0;
        Clear_Display();
        Print_Menu();
        Print_Temperature();
        break;
      }
      break;
    }



    case 3:
    {

      switch(Read_Keypad()){

        case 11:  

        Print_To_Screen("0",x++,7);
        enteredNumber = 1;
        while(Read_Keypad() == 11){}
          break;

        case 1:
        Print_To_Screen("1",x++,7);
        enteredNumber = 1;
        while(Read_Keypad() == 1){}
          break;

        case 2:

        Print_To_Screen("2",x++,7);
        enteredNumber = 1;
        while(Read_Keypad() == 2){}
          break;

        case 3:

        Print_To_Screen("3",x++,7);
        enteredNumber = 1;
        while(Read_Keypad() == 3){}
          break;

        case 4:

        Print_To_Screen("4",x++,7);
        while(Read_Keypad() == 4){}
          break;

        case 5:

        Print_To_Screen("5",x++,7);
        enteredNumber = 1;
        while(Read_Keypad() == 5){}
          break;


        case 6:

        Print_To_Screen("6",x++,7);
        enteredNumber = 1;
        while(Read_Keypad() == 6){}
          break;     


        case 7:

        Print_To_Screen("7",x++,7);
        enteredNumber = 1;
        while(Read_Keypad() == 7){}
          break;


        case 8:

        Print_To_Screen("8",x++,7);
        enteredNumber = 1;
        while(Read_Keypad() == 8){}
          break;

        case 9:

        Print_To_Screen("9",x++,7);
        enteredNumber = 1;
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
              Setup_Interrupts(1);
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
        }
        
        
      case 12:
        if(enteredNumber == 1 && enteredDot != 1){
          Setup_Interrupts(10);
          while(Read_Keypad() == 12){
          
            if(nInterrupts == 7){
              dotPosition = x;
              Print_To_Screen(".",x++,7);
              enteredDot = 1;
              while(Read_Keypad() == 12){}
              Setup_Interrupts(1);
              break;
            }
          }
          
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
  }

}


