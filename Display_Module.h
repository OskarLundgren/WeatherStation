#ifndef INCLUDE_DISPLAY_MODULE_H
#define INCLUDE_DISPLAY_MODULE_H
#include "Common_Variables.h"
#include "Common_Functions.h"

void Clear_Display();

unsigned char Read_Status_Display(void);
  
void Write_Data_2_Display(unsigned char data);

void Write_Command_2_Display(unsigned char command);

void Init_Display(void);

void setADP(int x,int y);

void Startup_Screen(void);

void Print_Menu(void);

void Print_Statistics(void);

void Print_To_Screen(char text[], int x, int y);

void Print_Menu_Attributes(void);

#endif