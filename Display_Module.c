#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Display_Module.h"
#include "Common_Functions.h"
#include "Temp_Module.h"
#include "Keypad_Module.h"

void Clear_Display(){

  setADP(0,0);
  for(int i = 0; i<640; i++){
  
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0xC0);
      
  }
  setADP(0,0);

}

unsigned char Read_Status_Display(void){
  
  unsigned char value;
  unsigned int theData;
  
  *AT91C_PIOD_SODR = buffer_keypad_OE;
  *AT91C_PIOC_ODR = databus;
  *AT91C_PIOC_SODR = buffer_display_dir;
  *AT91C_PIOC_SODR = buffer_display_OE; // Change buffer direction
  *AT91C_PIOC_SODR = CD; // Set register to command
  *AT91C_PIOC_CODR = CE; // Activate display chip
  *AT91C_PIOC_CODR = RD; // Activte read mode
  
  delay(25);
  
  theData = *AT91C_PIOC_PDSR;
  theData = theData >> 2; // Remove the first two zeros  
  value = theData & 0x3;
  
  *AT91C_PIOC_SODR = CE|RD;
  *AT91C_PIOC_CODR = buffer_display_dir;
  
  return value;
  
}

void Write_Data_2_Display(unsigned char data){

  while(Read_Status_Display() != 0x3){
  
  	delay(25);
    
  }
  
  *AT91C_PIOC_CODR = databus;
  *AT91C_PIOC_SODR = data << 2;
  *AT91C_PIOC_CODR = buffer_display_dir; // Write to display
  *AT91C_PIOC_CODR = buffer_display_OE;
  *AT91C_PIOC_OER = databus;
  *AT91C_PIOC_CODR = CD; // Set register to data
  *AT91C_PIOC_CODR = CE; // Activate display chip
  *AT91C_PIOC_CODR = WR; // Write data
  
  delay(25);
  
  *AT91C_PIOC_SODR = CE|WR|buffer_display_OE;
  *AT91C_PIOC_ODR = databus;

}

void Write_Command_2_Display(unsigned char command){
  
  *AT91C_PIOC_SODR = buffer_display_dir; // Change buffer direction
  
  while(Read_Status_Display() != 0x3){
  
    delay(25);
    
  }

  *AT91C_PIOC_CODR = databus;
  *AT91C_PIOC_SODR = command << 2;
  *AT91C_PIOC_CODR = buffer_display_dir; // Write to display
  *AT91C_PIOC_CODR = buffer_display_OE;
  *AT91C_PIOC_OER = databus;
  *AT91C_PIOC_SODR = CD; // Set register to command
  *AT91C_PIOC_CODR = CE; // Activate display chip
  *AT91C_PIOC_CODR = WR; // Write data
  
  delay(25);
  
  *AT91C_PIOC_SODR = CE; // Deactivate display chip
  *AT91C_PIOC_SODR = WR; // Read data
  *AT91C_PIOC_SODR = buffer_display_OE;
  *AT91C_PIOC_ODR = databus;  

}
  
void Init_Display(void){
  
  *AT91C_PMC_PCER = 1<<13|1<<14;
  *AT91C_PIOC_PER = buffer_display_dir|buffer_display_OE|CD|CE|WR|RD|RV;
  *AT91C_PIOD_PER = reset_display|buffer_keypad_OE;
  *AT91C_PIOC_ODR = 0x3C;

  *AT91C_PIOC_OER = buffer_display_dir|buffer_display_OE|CD|CE|WR|RD|RV;

  *AT91C_PIOC_CODR = RV;
  

  *AT91C_PIOD_CODR = reset_display;
  Setup_Interrupts(1000);
  while(nInterrupts < 10){}	//Use Systick for display 9ms
  
  *AT91C_PIOD_SODR = reset_display;
  
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40);
  
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x42);
  
  Write_Data_2_Display(0x28);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41);
    
  Write_Data_2_Display(0x28);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43);  
  
  Write_Command_2_Display(0x80);  
  
  Write_Command_2_Display(0x94);  
  
  Clear_Display();
}

void setADP(int x,int y){
  int address = x + (40*y);
  char lowAddress;
  char highAddress;
  
  lowAddress = address;
  address = address >> 8;
  highAddress = address;
  
  Write_Data_2_Display(lowAddress);
  Write_Data_2_Display(highAddress);
  Write_Command_2_Display(0x24);

}

void Startup_Screen(void){
  Print_To_Screen("Welcome",16,5);
  Setup_Interrupts(1000);
  while(nInterrupts < 1000){}
  Print_To_Screen("Initializing Modules",10,7);
  
  while(nInterrupts < 3000){}
  Clear_Display();

}

void Print_Menu(void){

   Print_To_Screen("1 - Statistics for the last 7 days",1,4);
   Print_To_Screen("2 - Find the orientation of the sun",1,6);
   Print_To_Screen("3 - Set a alarm for upper/lower temp",1,8);
   Print_To_Screen("4 - Enter fastmode (for simulation)",1,10);
   Print_To_Screen("5 - Number of samples/minute of temp",1,12);
   if(fastModeActivated == 1){
     Print_To_Screen("Hold * To Exit Fastmode",1,14);
   
   }
   
      

}

void Print_Statistics(void){
    int min = 1;
    int avg = 2;
    int max = 3;
    
    Print_To_Screen("(C)",1,4);
    
    Print_To_Screen("Mon", 6, 4);
    Convert_Day_To_Char(&weekBuffer[0],min);
    Print_To_Screen(converted, 6, 6);
    Convert_Day_To_Char(&weekBuffer[0],avg);            //Prints statistics related to Monday
    Print_To_Screen(converted, 6, 8);
    Convert_Day_To_Char(&weekBuffer[0],max);
    Print_To_Screen(converted, 6, 10);
    
    
    Print_To_Screen("Tue", 11, 4);
    Convert_Day_To_Char(&weekBuffer[1],min);
    Print_To_Screen(converted, 11, 6);
    Convert_Day_To_Char(&weekBuffer[1],avg);            //Prints statistics related to Tuesday
    Print_To_Screen(converted, 11, 8);
    Convert_Day_To_Char(&weekBuffer[1],max);
    Print_To_Screen(converted, 11, 10);
    
    
    Print_To_Screen("Wed", 16, 4);
    Convert_Day_To_Char(&weekBuffer[2],min);
    Print_To_Screen(converted, 16, 6);
    Convert_Day_To_Char(&weekBuffer[2],avg);            //Prints statistics related to Wednesday
    Print_To_Screen(converted, 16, 8);
    Convert_Day_To_Char(&weekBuffer[2],max);
    Print_To_Screen(converted, 16, 10);    
    
    
    Print_To_Screen("Thu", 21, 4);
    Convert_Day_To_Char(&weekBuffer[3],min);
    Print_To_Screen(converted, 21, 6);
    Convert_Day_To_Char(&weekBuffer[3],avg);            //Prints statistics related to Thursday
    Print_To_Screen(converted, 21, 8);
    Convert_Day_To_Char(&weekBuffer[3],max);
    Print_To_Screen(converted, 21, 10);    
    
        
    Print_To_Screen("Fri", 26, 4);
    Convert_Day_To_Char(&weekBuffer[4],min);
    Print_To_Screen(converted, 26, 6);
    Convert_Day_To_Char(&weekBuffer[4],avg);            //Prints statistics related to Friday
    Print_To_Screen(converted, 26, 8);
    Convert_Day_To_Char(&weekBuffer[4],max);
    Print_To_Screen(converted, 26, 10);      
    
    
    Print_To_Screen("Sat", 31, 4);
    Convert_Day_To_Char(&weekBuffer[5],min);
    Print_To_Screen(converted, 31, 6);
    Convert_Day_To_Char(&weekBuffer[5],avg);            //Prints statistics related to Saturday
    Print_To_Screen(converted, 31, 8);
    Convert_Day_To_Char(&weekBuffer[5],max);
    Print_To_Screen(converted, 31, 10);   
 
       
    Print_To_Screen("Sun", 36, 4);
    Convert_Day_To_Char(&weekBuffer[6],min);
    Print_To_Screen(converted, 36, 6);
    Convert_Day_To_Char(&weekBuffer[6],avg);            //Prints statistics related to Sunday
    Print_To_Screen(converted, 36, 8);
    Convert_Day_To_Char(&weekBuffer[6],max);
    Print_To_Screen(converted, 36, 10); 
    
       
    Print_To_Screen("Min:",1,6);
    Print_To_Screen("Avg:",1,8);
    Print_To_Screen("Max:",1,10);
}

void Print_Menu_Attributes(void){
  
  switch(currentMenu){
    
    case 0:
      Print_Menu();
      break;
    
    case 1:
      Print_To_Screen("Press * To Go Back",1,14);
      break;
      
    case 11:
      
      
      break;
      
    case 3:
       Print_To_Screen("What would you like to set a alarm for?",1,4);
       Print_To_Screen("1 - Lower Temperature",1,6);
       Print_To_Screen("2 - Higher Temperature",1,8);
       Print_To_Screen("3 - Both Higher and Lower Temperature",1,10);
       Print_To_Screen("4 - Delete Current Alarm",1,12);
       Print_To_Screen("Press * To Go Back",1,14);
       break;
       
    case 4:
        Print_To_Screen("Press 1 - Simulate A Day In 24 Minutes",1,4);
        Print_To_Screen("Press 2 - Simulate A Day In 60 Seconds",1,6);
        Print_To_Screen("Press * To Go Back",1,14);
        break;
        
        
    case 5:
      Print_To_Screen("Enter the number of samples/minute:",1,4);
      Print_To_Screen("(1-10)",1,6);
      Print_To_Screen("Press * To Go Back",1,14);
      if(Sample_Value != -1){
        Print_To_Screen("# = Confirm",28,14);
      
      }
      
      break;
        
  case 51:
      Print_To_Screen("Save Succesful",12,5);     
      break;
      
      
      
    case 31:
      
      if(justLower == 1 || both == 1){
        Print_To_Screen("Please Enter Lower Temperature:", 1,4);
      }
      if(justHigher == 1){
        Print_To_Screen("Please Enter Higher Temperature:",1,4);
      }
      
      if(isNegative != 1){
      Print_To_Screen("Hold * for (-)",1,12);
      }
      if(enteredNumber != 0 && enteredDot != 1){
      Print_To_Screen("Hold # for (.)",25,12);
      }
      Print_To_Screen("* = Delete", 1, 14);
      if(enteredNumber != 0){
      Print_To_Screen("          ",28,14);
      Print_To_Screen("# = Confirm", 28, 14);
      }
      else{
      Print_To_Screen("# = Cancel",28,14);
      }
      break;
      
  case 32:
    Print_To_Screen("Save Changes?",13,5);
    Print_To_Screen("* = No",8,9);
    Print_To_Screen("# = Yes",25,9);
    break;
  case 33:
    Print_To_Screen("Save Succesful",12,5);
    break;
    
  case 34:
    Print_To_Screen("Alarm Deleted",12,5);
    break;
    
  case 41:
     Print_To_Screen("Fastmode Entered",12,5);
  }
  

}


void Print_To_Screen(char text[], int x, int y){
  int i = 0;
  setADP(x,y);
  while(text[i] != '\0'){
      Write_Data_2_Display(text[i]-0x20);
      Write_Command_2_Display(0xC0);
      i++;
  }  
}