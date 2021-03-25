#ifndef __operation_H
#define __operation_H

#include "ALL_Includes.h"

//#define CCR_CUR_DETECT_LEVEL  230
#define CCR_CUR_DETECT_LEVEL  240
#define VBUF_VOLT_OFF_SET 150

#define INIT_DIMMING_VALUE 0 
#define MIN_DIMMING_VALUE  4
#define MAX_DIMMING_VALUE  800

#define VBUF_VOLT_CENTER 2400

#define DIMMING_RESET_VBUF_VOLT 1700
#define MAX_VBUF_VOLT 2700

#define MAX_PWM_BOOST 255
#define MIN_PWM_BOOST 10 // was 50 for default
#define INIT_PWM_BOOST 150

#define FLAG_RELAY_IS_NOT_CLOSED 0
#define FLAG_RELAY_IS_CLOSED 1

#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7C2)) 
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7B8)) 

#define MIN_FAULT_DETECT_VOLT 1500
#define MAX_FAULT_DETECT_VOLT 2800

#define TIME_OUT_FAULT_DETECT 2600  
//define TIME_OUT_FAULT_DETECT 5000  //5000

void find_dimming_val(void);
void update_led_dimming_control(void);
void update_boost_PWM_control(void);
void update_stability_res_control(void);
void update_mcu_temperature_val(void);
void update_vcc_volt_measure(void);
void update_fault_detect_a_volt(void);
void update_fault_detect_b_volt(void);
void check_fault_detect(void);
void update_relay_control(void);

#endif
