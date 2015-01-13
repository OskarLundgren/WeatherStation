
#ifndef INCLUDE_I2C_MODULE_H
#define INCLUDE_I2C_MODULE_H


#define Sensor_Ctrl 0x26
#define Sensor_Ctrl_SBYB 1
#define Sensor_Ctrl_Oversample_Ratio_2 1<<3
#define Sensor_PT_Data_CFG 0x13
#define Sensor_Pressure_Event_Flag 1<<1
#define Sensor_Pressure_Ready_Event_Flag 1<<2

void Init_I2C(void);

char Read_Sensor(char reg);

void Wait_For_Sensor(void);

void Write_To_Sensor(char registry, char data);

unsigned int Read_Pressure(void);



#endif