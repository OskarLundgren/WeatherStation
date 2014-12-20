#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Display_Module.h"
#include "Common_Functions.h"
#include "Temp_Module.h"

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

void Print_Menu(void){

   Print_To_Screen("1 - Statistics for the last 7 days",1,4);
   Print_To_Screen("2 - Find the orientation of the sun",1,6);
   Print_To_Screen("3 - Set a alarm for upper/lower temp",1,8);
   Print_To_Screen("4 - Enter fastmode (for simulation)",1,10);
   
      

}

void Print_Statistics(void){
    Print_To_Screen("(C)",1,4);
    Print_To_Screen("Mon", 6, 4);
    Print_To_Screen("Tue", 10, 4);
    Print_To_Screen("Wed", 14, 4);
    Print_To_Screen("Thu", 18, 4);
    Print_To_Screen("Fri", 22, 4);
    Print_To_Screen("Sat", 26, 4);
    Print_To_Screen("Sun", 30, 4);
    Print_To_Screen("Min:",1,6);
    Print_To_Screen("Avg:",1,8);
    Print_To_Screen("Max:",1,10);
}

void Print_Navigation_Buttons(void){
      
     Print_To_Screen("Press * To Go Back",1,14);
    
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
