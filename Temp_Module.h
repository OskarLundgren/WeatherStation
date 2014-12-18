#ifndef INCLUDE_TEMP_MODULE_H
#define INCLUDE_TEMP_MODULE_H

extern float temp;
extern int oneMinute;
extern int logTempPosition;
extern float tempLog[1440];
void Init_Temp(void);

void Temp_Measure(void);

void TC0_Handler(void);

void Update_Temp(void);

void Print_Temperature(void);

void Timer_Setup(void);

void TC3_Handler(void);

void Log_Temp(void);

int Size_Of_TempLog();
#endif