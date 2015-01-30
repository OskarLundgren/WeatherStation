#ifndef INCLUDE_COMMON_VARIABLES_H
#define INCLUDE_COMMON_VARIABLES_H
#include "system_sam3x.h"
#include "at91sam3x8.h"


#define buffer_keypad_OE ((unsigned int)1 << 2)
#define buffer_display_OE ((unsigned int)1 << 12)
#define buffer_display_dir ((unsigned int)1 << 13)
#define CD ((unsigned int)1 << 17)
#define CE ((unsigned int)1 << 16)
#define RD ((unsigned int)1 << 15)
#define WR ((unsigned int)1 << 14)
#define RV ((unsigned int)1 << 19)
#define databus ((unsigned int)0xFF << 2)
#define reset_display ((unsigned int)1)
#define rows ((unsigned int)0x3C)
#define columns ((unsigned int)0x380)


#define	ADC_MR ((volatile unsigned int*)0x400C0004)
#define	ADC_CHER ((volatile unsigned int*)0x400C0010)
#define	ADC_CR ((volatile unsigned int*)0x400C0000)
#define	ADC_IDR ((volatile unsigned int*)0x400C0028)
#define	ADC_LCDR ((volatile unsigned int*)0x400C0020)
#define	ADC_CDR2 ((volatile unsigned int*)0x400C0058)


#define PWM_ENA	((volatile unsigned int*)0x40094004)
#define PWM_CMR	((volatile unsigned int*)0x400942E0)
#define PWM_CPRD ((volatile unsigned int*)0x400942EC)
#define PWM_CDTY ((volatile unsigned int*)0x400942E4)
#define PWM_CDTYUPD ((volatile unsigned int*)0x400942E8)
#define PWM_CCNT ((volatile unsigned int*)0x400942F4)

#define fastMode ((unsigned int)(42000000/60)-1)
#define normalMode ((unsigned int) 42000000-1)

#define tempsensor ((unsigned int)1<<25)


#endif