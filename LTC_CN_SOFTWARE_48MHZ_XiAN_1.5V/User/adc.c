
#include "ALL_Includes.h"//包含所需的头文件

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define 	ADC1_DR_Address      0x40012440

__IO uint32_t TempSensVoltmv = 0, VrefIntVoltmv = 0;
__IO uint16_t RegularConvData_Tab[ADCHN];

void ADC_IO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* 使能GPIOA时钟 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

  /* 配置LED相应引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 使能GPIOB时钟 */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
  GPIO_Init(GPIOF, &GPIO_InitStructure);

}

void ADC1_CH_DMA_Config(void)
{
	ADC_InitTypeDef     ADC_InitStructure;
  DMA_InitTypeDef     DMA_InitStructure;
  
	/* ADC_Channel_0 IO DeInit */
	ADC_IO_Init();
	
  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  
	//RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div2); 
	
	
  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = ADCHN;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//????????
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//???
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//??ADC???????????
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;//??????????????????
	ADC_Init(ADC1, &ADC_InitStructure); 
 
  
	ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_13_5Cycles); // CCR CURRENT SENSING
	ADC_ChannelConfig(ADC1, ADC_Channel_2, ADC_SampleTime_13_5Cycles); // FAULT DETECT B
	ADC_ChannelConfig(ADC1, ADC_Channel_5, ADC_SampleTime_13_5Cycles); // VBUF volt
	ADC_ChannelConfig(ADC1, ADC_Channel_9, ADC_SampleTime_13_5Cycles); //FAULT DETECT A
	ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor,ADC_SampleTime_71_5Cycles); // temperature sensor 16channel
  ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint,ADC_SampleTime_71_5Cycles); // temperature sensor 17channel
	
	ADC_TempSensorCmd(ENABLE); 
	ADC_VrefintCmd(ENABLE);
	
  /* ADC?? */
	/*
	 *     @arg ADC_SampleTime_1_5Cycles: Sample time equal to 1.5 cycles  
  *     @arg ADC_SampleTime_7_5Cycles: Sample time equal to 7.5 cycles
  *     @arg ADC_SampleTime_13_5Cycles: Sample time equal to 13.5 cycles
  *     @arg ADC_SampleTime_28_5Cycles: Sample time equal to 28.5 cycles
  *     @arg ADC_SampleTime_41_5Cycles: Sample time equal to 41.5 cycles
  *     @arg ADC_SampleTime_55_5Cycles: Sample time equal to 55.5 cycles
  *     @arg ADC_SampleTime_71_5Cycles: Sample time equal to 71.5 cycles
  *     @arg ADC_SampleTime_239_5Cycles: Sample time equal to 239.5 cycles
	*/
	
  ADC_GetCalibrationFactor(ADC1);
  
	  /* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);  
	
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* ??ADRDY falg */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
	
	//printf("ADC1 Start\r\n");
}

