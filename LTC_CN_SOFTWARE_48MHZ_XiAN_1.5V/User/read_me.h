#ifndef _READ_ME_H_
#define _READ_ME_H_

/******************************************************************************************
*date : 2018-11-05
*version : 1.055
*by : YMY
*location : 1, if((vbuf_volt >= 3400)&&(boost_pwm_value < MAX_PWM_BOOST))
					       boost_pwm_value++ ;
						   void update_boost_PWM_control(void);
						2, ����MAX_PWM_BOOST/MIN_PWM_BOOST/INIT_PWM_BOOST
						       FLAG_RELAY_IS_NOT_CLOSED/FLAG_RELAY_IS_CLOSED
									 TEMP110_CAL_ADDR/TEMP30_CAL_ADDR
									 MIN_FAULT_DETECT_VOLT/MAX_FAULT_DETECT_VOLT
									 TIME_OUT_FAULT_DETECT 
									 λ��Ų��operation.h��
							 ͬʱɾ��STEP_B1��
						3��ɾ��û���õĺ���update_CCR_current_val();
*reason : 1, VBUF����34Vʱ��BOOST_PWM���ܻᳬ��Χ
******************************************************************************************/

/******************************************************************************************
*date : 2018-08-11
*version : 1.053
*by : YMY
*location : start_up_counter < 5, balance fast_loop_counter%10 -> fast_loop_counter%5
            operation.c -> void update_stability_res_control(void)
*reason : 1, 1���⿪������˸
******************************************************************************************/


/******************************************************************************************
*date : 2018-01-24
*version : 1.052
*by : YMY
*location :	BOOST_PWM regulation speed: %50, DIMMING++ speed: %100
*reason : 1, ����5���⣬����ת����5���⣨123��  ������˸
          2��full load���߿�·30%��ʱ�����ȶ���������3ohm����
******************************************************************************************/

/******************************************************************************************
*date : 2018-01-23
*version : 1.051-test1
*by : YMY
*location :	BOOST_PWM regulation speed: %50
*result : 1,������40��˫·LTC�������⼶�����͹⼶ת�������ȶ���Ψһ�����⣬�漰��5�����ʱ�򣬻�����˸��
          2����·12��˫·LTC�������⼶�����͹⼶ת�������ȶ���Ψһ�����⣬�漰��5�����ʱ�򣬻�����˸��
******************************************************************************************/


/******************************************************************************************
*date : 2018-01-17
*version : 1.051
*by : YMY
*location :	FDT counter��5000���2600��
*reason : 
******************************************************************************************/

/******************************************************************************************
*date : 2018-01-15
*version : 1.05
*by : YMY
*location : FDT counter����������������һ����������VBUF>(VBUF_CENTER - VBUF_OFF_SET),����VBUFС��22Vʱ��������FDT�ж���
            �������������2.4A��С��2.7Aʱ��DIMMING���䣬�������4����Ϊ4��
						��DIMMING valueС��4ʱ��FDT counter���㣻
						FDT counter��2600���5000��
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


