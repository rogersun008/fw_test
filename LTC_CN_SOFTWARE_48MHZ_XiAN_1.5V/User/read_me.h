#ifndef _READ_ME_H_
#define _READ_ME_H_

/******************************************************************************************
*date : 2018-11-05
*version : 1.055
*by : YMY
*location : 1, if((vbuf_volt >= 3400)&&(boost_pwm_value < MAX_PWM_BOOST))
					       boost_pwm_value++ ;
						   void update_boost_PWM_control(void);
						2, 定义MAX_PWM_BOOST/MIN_PWM_BOOST/INIT_PWM_BOOST
						       FLAG_RELAY_IS_NOT_CLOSED/FLAG_RELAY_IS_CLOSED
									 TEMP110_CAL_ADDR/TEMP30_CAL_ADDR
									 MIN_FAULT_DETECT_VOLT/MAX_FAULT_DETECT_VOLT
									 TIME_OUT_FAULT_DETECT 
									 位置挪到operation.h中
							 同时删除STEP_B1等
						3，删除没有用的函数update_CCR_current_val();
*reason : 1, VBUF大于34V时，BOOST_PWM可能会超范围
******************************************************************************************/

/******************************************************************************************
*date : 2018-08-11
*version : 1.053
*by : YMY
*location : start_up_counter < 5, balance fast_loop_counter%10 -> fast_loop_counter%5
            operation.c -> void update_stability_res_control(void)
*reason : 1, 1级光开机有闪烁
******************************************************************************************/


/******************************************************************************************
*date : 2018-01-24
*version : 1.052
*by : YMY
*location :	BOOST_PWM regulation speed: %50, DIMMING++ speed: %100
*reason : 1, 开机5级光，或者转换到5级光（123）  会有闪烁
          2，full load或者开路30%的时候都能稳定，串联了3ohm电阻
******************************************************************************************/

/******************************************************************************************
*date : 2018-01-23
*version : 1.051-test1
*by : YMY
*location :	BOOST_PWM regulation speed: %50
*result : 1,满负载40个双路LTC，各个光级开机和光级转换都能稳定。唯一的问题，涉及到5级光的时候，会有闪烁。
          2，开路12个双路LTC，各个光级开机和光级转换都能稳定。唯一的问题，涉及到5级光的时候，会有闪烁。
******************************************************************************************/


/******************************************************************************************
*date : 2018-01-17
*version : 1.051
*by : YMY
*location :	FDT counter由5000变成2600；
*reason : 
******************************************************************************************/

/******************************************************************************************
*date : 2018-01-15
*version : 1.05
*by : YMY
*location : FDT counter递增条件，加上了一个条件：当VBUF>(VBUF_CENTER - VBUF_OFF_SET),即当VBUF小于22V时，不进行FDT判定；
            当输入电流大于2.4A，小于2.7A时，DIMMING不变，如果大于4，即为4；
						当DIMMING value小于4时，FDT counter归零；
						FDT counter由2600变成5000；
*reason : 

*location : operation.c -> #define MIN_FAULT_DETECT_VOLT 1700
*reason : when one LED is shorted, FDT_A or FDT_B value is between 1500 and 1600, MIN_FAULT_DETECT_VOLT is set more than 1600
******************************************************************************************/


/******************************************************************************************
*date : 2017-09-07
*version : 1.048
*by : YMY
*location : operation.c -> void update_led_dimming_control(void)
          ->if(flag_fault_detect == FLAG_LED_IS_FAULT)      
			        current_dimming_value = 0 ;
*reason : when one or more LEDs are shorted, shut down all the LEDs, this is for non-monitor models

*location : operation.c -> #define MIN_FAULT_DETECT_VOLT 1700
*reason : when one LED is shorted, FDT_A or FDT_B value is between 1500 and 1600, MIN_FAULT_DETECT_VOLT is set more than 1600
******************************************************************************************/


/******************************************************************************************
*date : 2017-08-16
*version : 1.047
*by : YMY
*location : operation.c -> void find_dimming_val(void)
          ->input_current_rms_int = (input_current_rms_100*rate_current + input_current_rms_previous*(100-rate_current))/100;
*reason : caculate the input current as a moving window to make it change smoothly,
          then the dimming value will be changed smoothly, and led strip has no visible flickering.
******************************************************************************************/

#endif


