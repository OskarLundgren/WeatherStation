#include "Common_Functions.h"
#include "I2C_Module.h"


void Init_I2C(void){
  /*Here we enable the clock for the Two-Wired Interface (TWI). We also let the TWI control PA18 and PA19*/

  *AT91C_PMC_PCER = 1<<22|1<<11; 
  *AT91C_PIOA_PDR = 1<<17|1<<18;
  
  /*Here we set our Arduino to be the master in this TWI. We also write the address to the sensor in the TWI_MMR register.*/
  
  *AT91C_TWI0_CWGR = 0x60606;
  *AT91C_TWI0_CR = AT91C_TWI_SVDIS|AT91C_TWI_MSEN;
  *AT91C_TWI0_MMR = 0x60<<16|AT91C_TWI_IADRSZ_1_BYTE;
  
  Write_To_Sensor(Sensor_Ctrl,Sensor_Ctrl_SBYB|Sensor_Ctrl_Oversample_Ratio_2); //Turns on the sensor
  Write_To_Sensor(Sensor_PT_Data_CFG,Sensor_Pressure_Event_Flag|Sensor_Pressure_Ready_Event_Flag);
  
}

char Read_Sensor(char reg){
   char TWI_Value;
   
   Wait_For_Sensor();
   
   *AT91C_TWI0_MMR |= AT91C_TWI_MREAD;
   *AT91C_TWI0_IADR = reg;
   *AT91C_TWI0_CR = AT91C_TWI_START|AT91C_TWI_STOP;
    int timeout = 0;
    while(!(*AT91C_TWI0_SR&AT91C_TWI_RXRDY) && timeout < 10000){
          timeout ++;
  }
  
  TWI_Value = *AT91C_TWI0_RHR;
  return TWI_Value;

}


unsigned int Read_Pressure(void){
  unsigned int Pressure_Value;
  unsigned char MSB = Read_Sensor(0x1);
  unsigned char CSB = Read_Sensor(0x2);
  unsigned char LSB = Read_Sensor(0x3);
  Pressure_Value = (MSB<<10)|(CSB<<2)|(LSB>>6);
  
  return Pressure_Value;
    

}


void Write_To_Sensor(char reg, char data){

    Wait_For_Sensor();
    
    
    *AT91C_TWI0_MMR &= ~AT91C_TWI_MREAD;
    *AT91C_TWI0_IADR = reg;
    *AT91C_TWI0_THR = data;
    *AT91C_TWI0_CR = AT91C_TWI_STOP;
    int timeout = 0;
    while(!(*AT91C_TWI0_SR&AT91C_TWI_TXRDY_MASTER) && timeout < 10000){
		timeout ++;
	}
    
}


void Wait_For_Sensor(void){
    
  int timeout = 0;
  while(!(*AT91C_TWI0_SR&AT91C_TWI_TXCOMP_MASTER) && timeout < 1000){
          timeout ++;
	}


}

