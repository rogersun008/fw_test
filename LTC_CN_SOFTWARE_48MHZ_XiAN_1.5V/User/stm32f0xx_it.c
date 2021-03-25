/**
  ******************************************************************************
  * @file    EXTI/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-October-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "ALL_Includes.h"//包含所需的头文件

/** @addtogroup STM32F0308_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup EXTI_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 tmp = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define MAX_ADC_SAMPLING_COUNTER 2048
//#define MAX_ADC_SAMPLING_COUNTER 1024

extern unsigned char fault_detect_a;
extern unsigned int vbuf_volt;
extern  unsigned int vcc_mcu_real_volt ;
unsigned long input_current_rms;

//extern float input_current_rms_f  ;
extern unsigned long input_current_rms_100 ;
unsigned int adc_sampling_counter = 1;
unsigned long input_current_rms_rms_sum_this_cycle = 0;
unsigned long input_current_rms_rms = 0;
unsigned long input_current_rms_rms_sum[8] = {0 , 0 , 0 , 0} ;

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0x8.s).                                               */
/******************************************************************************/


/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
  {
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) == KEY_ON ) 
	  {
			//delay_ms(30);				//如果是按键需要加防抖
			//if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) == KEY_ON ) 
			fault_detect_a = 1;
			//LED_ON();
		}
		else
		{
			//delay_ms(30);				//如果是按键需要加防抖
			//if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) == KEY_OFF ) 
			fault_detect_a = 0;
			//LED_OFF();
		}
  }
	EXTI_ClearITPendingBit(EXTI_Line3);
}


void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	unsigned int temp_val_1 = 0 ;
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		if(adc_sampling_counter <= MAX_ADC_SAMPLING_COUNTER)
		{
			temp_val_1 = RegularConvData_Tab[0] >> 3 ; // convert ADC value from 12bit to 9bit
			input_current_rms_rms_sum_this_cycle += temp_val_1 * temp_val_1;
		}
		
		if(adc_sampling_counter == MAX_ADC_SAMPLING_COUNTER)
		{
			input_current_rms_rms_sum[7] = input_current_rms_rms_sum[6];
			input_current_rms_rms_sum[6] = input_current_rms_rms_sum[5];
			input_current_rms_rms_sum[5] = input_current_rms_rms_sum[4];
			input_current_rms_rms_sum[4] = input_current_rms_rms_sum[3];
			input_current_rms_rms_sum[3] = input_current_rms_rms_sum[2];
			input_current_rms_rms_sum[2] = input_current_rms_rms_sum[1];
			input_current_rms_rms_sum[1] = input_current_rms_rms_sum[0];
			input_current_rms_rms_sum[0] = input_current_rms_rms_sum_this_cycle;
			
			input_current_rms_rms = (input_current_rms_rms_sum[0]
															+ input_current_rms_rms_sum[1]
															+ input_current_rms_rms_sum[2]
															+ input_current_rms_rms_sum[3]
															+ input_current_rms_rms_sum[4]
															+ input_current_rms_rms_sum[5]
															+ input_current_rms_rms_sum[6]
															+ input_current_rms_rms_sum[7])>>6; // 10 ;			//平均值
			
			input_current_rms = sqrt(input_current_rms_rms);							//开方
			input_current_rms_rms_sum_this_cycle = 0;
			adc_sampling_counter  = 0;
		}
		
		input_current_rms_100 = (input_current_rms * vcc_mcu_real_volt) ;// - 60000 ;
		
		input_current_rms_100 = input_current_rms_100/3210; // 1598 ;
		//	input_current_rms_100 = 660; // 1598 ;
		
		if(adc_sampling_counter < MAX_ADC_SAMPLING_COUNTER)		
		{
			adc_sampling_counter ++;  
		}
	}
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//清除中断标志	 
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{

	}

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除中断标志	 
}
//
void TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
	{

	}
	TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
}

//=============================================================================
//文件名称：
//功能概要：USART1中断函数
//参数说明：无
//函数返回：无
//=============================================================================
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
   //USART_SendData(USART1,USART_ReceiveData(USART1));
	 while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
