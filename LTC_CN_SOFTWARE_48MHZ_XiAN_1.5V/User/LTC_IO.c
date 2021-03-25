#include "LTC_IO.h"
#include "ALL_Includes.h"


extern unsigned char fault_detect_a;

/****************************************************
函数功能：LTC_IO初始化
输入参数：无
输出参数：无
备    注：
****************************************************/
void LTC_IO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOF, ENABLE ) ; //| RCC_AHBPeriph_GPIOB, ENABLE);
		//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF , ENABLE ) ;
	
		//PB1:数字信号输入,fault_detect_a
	/*
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/
		//PA0:relay_control,PA3:heart_beat_led,PA7:fuse_open
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  //PA10: Detect working mode
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//PF0 : relay open control
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/****************************************************
函数功能：LED开
输入参数：无
输出参数：无
备    注：调用此函数前，需要在LED.h修改宏定义LED引脚
****************************************************/
void LED_ON(void)
{
		GPIO_ResetBits(LED_PORT, LED_PIN);
}

/****************************************************
函数功能：LED关
输入参数：无
输出参数：无
备    注：调用此函数前，需要在LED.h修改宏定义LED引脚
****************************************************/
void LED_OFF(void)
{
		GPIO_SetBits(LED_PORT, LED_PIN);
}

void set_relay_open_coil(void)
{
		GPIO_SetBits(GPIOF, GPIO_Pin_0);
}

void clr_relay_open_coil(void)
{
		GPIO_ResetBits(GPIOF, GPIO_Pin_0);
}

void set_relay_close_coil(void)
{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void clr_relay_close_coil(void)
{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void heart_beat_led_on(void)
{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
}
void heart_beat_led_off(void)
{
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
}

void fuse_open_enable(void)
{
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
}
void fuse_open_disable(void)
{
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
}

//刷新fault_detect_a
void fault_detect(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == KEY_ON ) 
	{
		fault_detect_a = 1;
	}
	else
	{
		fault_detect_a = 0;
	}
}

