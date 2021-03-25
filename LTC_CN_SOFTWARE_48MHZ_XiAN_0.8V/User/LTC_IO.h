#ifndef __LTC_IO_H
#define __LTC_IO_H 

#include "ALL_Includes.h"

#define LED_GPIO_CLK   RCC_AHBPeriph_GPIOA
#define LED_PORT   	   GPIOA
#define LED_PIN        GPIO_Pin_4



void LTC_IO_Init(void);
void LED_ON(void);
void LED_OFF(void);

void set_relay_open_coil(void);
void clr_relay_open_coil(void);
void set_relay_close_coil(void);
void clr_relay_close_coil(void);

void heart_beat_led_on(void);
void heart_beat_led_off(void);
void fuse_open_enable(void);
void fuse_open_disable(void);
void fault_detect(void);

#endif



