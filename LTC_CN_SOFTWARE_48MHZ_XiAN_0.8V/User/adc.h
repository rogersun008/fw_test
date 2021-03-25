#ifndef __ADC_H
#define	__ADC_H

#include "stm32f0xx.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_dma.h"
		 
#define ADCHN  				6 	//通道数
#define ADSAMPLENUM  	10 	//采集次数

extern	__IO uint32_t TempSensVoltmv, VrefIntVoltmv;
extern	__IO uint16_t RegularConvData_Tab[ADCHN];
extern  unsigned int vref_cal_int ;

void ADC_IO_Init(void);
void ADC1_CH_DMA_Config(void);

#endif /* __ADC_H */



