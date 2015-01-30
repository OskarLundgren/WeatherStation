#ifndef INCLUDE_TEMP_MODULE_H
#define INCLUDE_TEMP_MODULE_H

extern float temp;
extern int nSeconds;
extern int tempLogPosition;
extern float tempLog[1440];
extern char converted[4];
extern int currentMenu;
extern int nSample;
extern int Minute_Sample_Value;
extern int Time_To_Log;
extern int Time_To_Add_Value;

typedef struct day{
  
  float minValue;
  float maxValue;
  float avgValue;
  
  
} Day;

typedef struct minute{
  
  float Temp_Value;
  struct minute *next;
  
  
} Minute_Node;

extern Day weekBuffer[7];

void Init_Temp(void);

void Temp_Measure(void);

void TC0_Handler(void);

void Update_Temp(void);

void Clear_Temp(void);

void Print_Temperature(void);

void Timer_Setup(void);

void TC3_Handler(void);

void Log_Temp(void);

void Log_Minute_Average(float Log[], int Array_Length);

int Size_Of_TempLog();

void Add_Values(void);

void Reset_TempLog(void);

float Find_Max();

float Find_Min();

float Find_Average();

void Convert_Day_To_Char(Day* theDay, int requestedValue);

void Temp_Alarm(void);

#endif