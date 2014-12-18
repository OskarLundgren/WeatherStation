#ifndef INCLUDE_LIGHT_MODULE_H
#define INCLUDE_LIGHT_MODULE_H

void Light_Init(void);

void ADC_Setup(void);

void PWM_Setup(void);

unsigned short Light_Measure(void);

void Find_Light(void);

#endif