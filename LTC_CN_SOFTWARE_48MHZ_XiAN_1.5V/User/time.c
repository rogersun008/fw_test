
#include "ALL_Includes.h"//包含所需的头文件

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;

//timer1 to generate 20KHZ intteruption for adc RMS calculation

void TIM1_Config(void)
{	 
	NVIC_InitTypeDef NVIC_InitStructure;
	/*  TIM1 ??????*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE); 
	TIM1->PSC= 479;//???? 48M / 80 / 6 = 10us
	TIM1->ARR= 4;		//?? 50us,20KHz
	TIM1->DIER|=1<<0;//??????	
	TIM1->CR1|=1<<4;//????
	TIM1->CR1|=0x01;  //?????1 
}

void TIM3_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE ) ; //| RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
}

//???3 ,TIM3_CH1,12?,PA6,200Hz
void TIM3_PWM_Config(void)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*Enable or disable the AHB peripheral clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 
	TIM3->PSC= 15 ;					//???? = 48M * 256 / 1024 / 6 = 2us
	TIM3->ARR= 799 ; //PWM3_FULL-1;		//?? 1 / 256us = 3.9KHz
	//============PWM1
	TIM3->CCER|=1<<0;				//OC1????
	TIM3->CCMR1|=0x0060;
	TIM3->CCR1=0;
	TIM3->CCMR1|=0x0004;
	
	TIM3->CR1|=1<<7;				//ARPE
	TIM3->CR1|=0x01;  			//?????3 
}


//???14 ,TIM14_CH1,10?,PA4,62.5KHz
void TIM14_PWM_Config(void)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*Enable or disable the AHB peripheral clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14 , ENABLE); 
	TIM14->PSC=2;							//???? 48*(1000/62.5)/128/6 = 1
	TIM14->ARR=PWM14_FULL-1;	//??  62.5KHZ
	//============PWM1
	TIM14->CCER|=1<<0;				//OC1????
	TIM14->CCMR1|=0x0060;
	//TIM14->CCR1=128;
	TIM14->CCR1= 0 ;
	TIM14->CCMR1|=0x0004;
	
	TIM14->CR1|=1<<7;					//ARPE
	TIM14->CR1|=0x01;  				//?????3 
}


void dimming_control(unsigned int dimming_duty)
{
	if(dimming_duty < PWM3_FULL + 1)
	    TIM3->CCR1 = dimming_duty;
}

void boost_pwm_control(unsigned int boost_pwm_duty) 
{
	if(boost_pwm_duty < PWM14_FULL + 1)
	    TIM14->CCR1 = boost_pwm_duty;
}

