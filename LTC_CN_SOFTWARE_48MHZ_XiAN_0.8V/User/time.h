#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f0xx.h"
//��ʱ��1 ,20KHz
void TIM1_Config(void);
//��ʱ��3 ,TIM3_CH1��12�ţ�PA6,200Hz
void TIM3_PWM_Config(void);
//��ʱ��14 ,TIM14_CH1��10�ţ�PA4,62.5Hz
void TIM14_PWM_Config(void);

void dimming_control(unsigned int dimming_duty);
void boost_pwm_control(unsigned int boost_pwm_duty) ;

void TIM3_GPIO_Config(void);

#define  PWM3_FULL    800
#define  PWM14_FULL    256

#endif  /* _TIME_H_ */


