#include "ALL_Includes.h"
#include "dimming_curve.h"

#define FLAG_LED_IS_FAULT 1
#define FLAG_LED_IS_NOT_FAULT 0

#define STATUS_RELAY_CLOSE_COIL_POWERED 1
#define STATUS_RELAY_CLOSE_COIL_NOT_POWERED 0

extern unsigned long input_current_rms_100 ;
unsigned int target_dimming_value = 0 ; 
extern unsigned int main_loop_counter ;
extern unsigned int main_fast_loop_counter ;
unsigned int current_dimming_value = INIT_DIMMING_VALUE  ;
unsigned int vbuf_volt = 0;
unsigned int relay_close_coil_powered_counter = 0;
unsigned int relay_close_coil_not_powered_counter = 0;
unsigned char boost_pwm_value  = 200 ;
//unsigned char flag_relay_status = FLAG_RELAY_IS_NOT_CLOSED ;
unsigned int mcu_temperature_val = 0 ;
unsigned int vcc_mcu_real_volt = 3400  ;

unsigned int vcc_mcu_real_volt_arr[128]  ;
unsigned char counter_vcc_mcu_measure = 0  ;
unsigned char relay_close_coil_status = STATUS_RELAY_CLOSE_COIL_POWERED  ;
unsigned long sum_vcc_mcu_measure = 0  ;
unsigned int fault_detect_a_val = 0  ;
unsigned int fault_detect_b_val = 0  ;
unsigned int counter_fault_a_detect = 0 ;
unsigned int counter_fault_b_detect = 0 ;
unsigned char flag_fault_detect = FLAG_LED_IS_NOT_FAULT ;

extern unsigned char start_up_counter;
unsigned char which_step = 0;

unsigned long input_current_rms_previous = 0;
unsigned long input_current_rms_compared = 0;
unsigned char rate_current = 100;

void find_dimming_val(void)
{
	unsigned int input_current_rms_int = 0 ;
	unsigned int input_current_change = 0;
	unsigned int dimming_curve_location = 0 ;
	float temp_float = 0 ;
	
	input_current_rms_int = (input_current_rms_100*rate_current + input_current_rms_previous*(100-rate_current))/100;
	  
	rate_current -= 10;
	if(rate_current<=10)
		rate_current = 10;
	input_current_rms_previous = input_current_rms_int ;
		
	if(input_current_rms_int >= input_current_rms_compared)
		input_current_change = input_current_rms_int - input_current_rms_compared;
	else
		input_current_change = input_current_rms_compared - input_current_rms_int;

	if(input_current_rms_100 <= 220)  
		current_dimming_value = 0 ;
		
	if((input_current_rms_100 <= 240)&&(input_current_rms_100 > 220)) 
	{
		current_dimming_value = current_dimming_value ;
		if(current_dimming_value >= 4)
			current_dimming_value = 4 ;
	}

	if((input_current_rms_100 < 270)&&(input_current_rms_100 > 240)) 
		current_dimming_value = 4;
		
	if(input_current_rms_int >= 670) // if CCR current hgiher than 6.7A , dimming value set to maximum
		target_dimming_value = MAX_DIMMING_VALUE ;
		
	if((input_current_rms_int >= 270)
		&&(input_current_rms_int < 670)
		&&(input_current_change > 20))   //if input current has a change more than 0.2A
	{
		dimming_curve_location = input_current_rms_int - 270 ; 
		temp_float = (dimming_curve_color[dimming_curve_location] * MAX_DIMMING_VALUE)/100;
		target_dimming_value = temp_float ;	    
    input_current_rms_compared = input_current_rms_int;		
	}
		
	if((input_current_rms_int >= 270)
		&&(input_current_rms_int < 670)
		&&(input_current_change <= 20))   //if input current has a change less than 0.2A
	{
		dimming_curve_location = input_current_rms_compared - 270 ; 
		temp_float = (dimming_curve_color[dimming_curve_location] * MAX_DIMMING_VALUE)/100;
		target_dimming_value = temp_float ;	    	
	}	
}

void update_led_dimming_control(void)
{
	unsigned char temp = 0;

	if((current_dimming_value < target_dimming_value )
		&&(current_dimming_value < MAX_DIMMING_VALUE)
	  &&(main_fast_loop_counter % 100 == 0))
	{
		current_dimming_value ++ ;
	}
			
	if((current_dimming_value > target_dimming_value )
		&&(current_dimming_value > MIN_DIMMING_VALUE))
	{
		current_dimming_value -- ;
	}
			
	if((vbuf_volt < 1900)
		&&(current_dimming_value > MIN_DIMMING_VALUE))
	{
		temp = current_dimming_value>>6;
		current_dimming_value -= temp ;
	}	
			
	if((main_loop_counter%1 == 0)
		&&(vbuf_volt > 3400 )
		&&(current_dimming_value < MAX_DIMMING_VALUE))
	{
		current_dimming_value ++ ;
	}

  if(vbuf_volt < DIMMING_RESET_VBUF_VOLT)
	{
		current_dimming_value = MIN_DIMMING_VALUE;
	}
			
	if(input_current_rms_100 <= 220)  
		current_dimming_value = 0 ;
		
	if((input_current_rms_100 < 270)&&(input_current_rms_100 > 220)) 
	{
		current_dimming_value = current_dimming_value ;
		if(current_dimming_value >= 4)
			current_dimming_value = 4;
	}
		
	//when one or more LED shorted, shut down all other LEDs, this is reference for non-monitor model
	if(flag_fault_detect == FLAG_LED_IS_FAULT)      
		current_dimming_value = 0 ;
		
	dimming_control(current_dimming_value);		//200Hz,0-1024	
}

void update_boost_PWM_control(void)
{		
	vbuf_volt = (RegularConvData_Tab[2]*vcc_mcu_real_volt)/3724;
		
	if(main_fast_loop_counter % 50 == 0)
	{
		if((vbuf_volt > (VBUF_VOLT_CENTER + VBUF_VOLT_OFF_SET))
			&&(boost_pwm_value < MAX_PWM_BOOST))
			boost_pwm_value ++ ;
			
		if((vbuf_volt < (VBUF_VOLT_CENTER - VBUF_VOLT_OFF_SET))
			&&(boost_pwm_value > MIN_PWM_BOOST))
			boost_pwm_value --  ;
	}

	if(main_loop_counter%1 == 0) // corresponding to 24VDC .
	{
			/*
			if(vbuf_volt >= 2600)
					boost_pwm_value++ ;
			
			if(vbuf_volt >= 2700)
					boost_pwm_value++ ;
			
			if(vbuf_volt >= 2800)
					boost_pwm_value++ ;
			*/
			if((vbuf_volt >= 3400)&&(boost_pwm_value < MAX_PWM_BOOST))
					boost_pwm_value++ ;
	}	
	boost_pwm_control(boost_pwm_value);				//62.5KHz,0-256
}

void update_stability_res_control(void)
{		
	if(main_fast_loop_counter%1 == 0) // corresponding to 24VDC .
	{
		if(vbuf_volt >= 3000)
			heart_beat_led_off();
		else
			heart_beat_led_on();	
	}	
		
	if((main_fast_loop_counter%10 == 0)
     &&(vbuf_volt >= VBUF_VOLT_CENTER))
		 heart_beat_led_off();
	else
		 heart_beat_led_on();	
	
	if(start_up_counter < 5)
	{
		if((main_fast_loop_counter%5 == 0)          //20180811  add
     &&(vbuf_volt >= VBUF_VOLT_CENTER))
			heart_beat_led_off();
	  else
			heart_beat_led_on();
	}
	
	if(vbuf_volt < 500) // corresponding to 24VDC .
	{
		if(main_loop_counter < 500)
			heart_beat_led_off();
		else
			heart_beat_led_on();	
	}
}

void update_mcu_temperature_val(void)
{
	unsigned int temp_int ;
	unsigned long temp_long ;
	unsigned int temp_sensor_val_3300 ;
	
	temp_sensor_val_3300 = (RegularConvData_Tab[4] * vcc_mcu_real_volt) /3300;
	
	temp_long = (int32_t) *TEMP30_CAL_ADDR - (int32_t) *TEMP110_CAL_ADDR ;
	temp_long = temp_long * 100 ;
	temp_int = temp_long/80 ; // get voltage/temperature ratio
	
	if(temp_sensor_val_3300 >= (int32_t) *TEMP110_CAL_ADDR)
	{
		temp_long = (temp_sensor_val_3300 - (int32_t) *TEMP110_CAL_ADDR)*100 ;
		temp_int = temp_long/temp_int;
		temp_int = 110 - temp_int ;
	}

	if(temp_sensor_val_3300 < (int32_t) *TEMP110_CAL_ADDR)
	{
		temp_long = ((int32_t) *TEMP110_CAL_ADDR - temp_sensor_val_3300)*100;
		temp_int = temp_long/temp_int;
		temp_int = 110 + temp_int ;
	}
	mcu_temperature_val = temp_int ;
}

void update_vcc_volt_measure(void)
{
	unsigned int temp_int ;
	unsigned long temp_long ;	
	unsigned int vref_int_cal ;
	if(main_loop_counter%5 == 0)
	{
		vref_int_cal = *(__IO uint16_t *)(0X1FFFF7BA);
		temp_int = vref_int_cal ; 
		temp_long = temp_int * 33000 ;
		temp_int = temp_long / RegularConvData_Tab[5] / 10;

		if(counter_vcc_mcu_measure < 128)
		{
			sum_vcc_mcu_measure += temp_int ; 
			counter_vcc_mcu_measure++ ;
		}
		
		if(counter_vcc_mcu_measure == 128)
		{
			temp_int = sum_vcc_mcu_measure >> 7 ; 
			sum_vcc_mcu_measure = 0 ;
			counter_vcc_mcu_measure= 0 ;
			vcc_mcu_real_volt = temp_int ;
		}
	}
}

void update_fault_detect_a_volt(void)
{
	unsigned int temp_here = 0;
	temp_here = (RegularConvData_Tab[3] * vcc_mcu_real_volt )/4096 ;
	
	if(current_dimming_value < 10)
	{
		if(temp_here > 50)
			fault_detect_a_val = temp_here - 50 ;
		else
			fault_detect_a_val = 0 ;
	}
	
	if((current_dimming_value >= 10)
		&&(current_dimming_value < 20))
	{	
		if(temp_here > 30)
			fault_detect_a_val = temp_here - 30 ;
		else
			fault_detect_a_val = 0 ;
	}
	
	if((current_dimming_value >= 20)
		&&(current_dimming_value < 80))
	{	
		if(temp_here > 20)
			fault_detect_a_val = temp_here - 20 ;
		else
			fault_detect_a_val = 0 ;
	}
	
	if(current_dimming_value >= 80)
		fault_detect_a_val = temp_here ;
}

void update_fault_detect_b_volt(void)
{
	unsigned int temp_here = 0;
	temp_here = (RegularConvData_Tab[1] * vcc_mcu_real_volt )/4096 ;
		if(current_dimming_value < 10)
	{
		if(temp_here > 50)
			fault_detect_b_val = temp_here - 50 ;
		else
			fault_detect_b_val = 0 ;
	}
	
	if((current_dimming_value >= 10)
		&&(current_dimming_value < 20))
	{	
		if(temp_here > 30)
			fault_detect_b_val = temp_here - 30 ;
		else
			fault_detect_b_val = 0 ;
	}
	
	if((current_dimming_value >= 20)
		&&(current_dimming_value < 80))
	{	
		if(temp_here > 20)
			fault_detect_b_val = temp_here - 20 ;
		else
			fault_detect_b_val = 0 ;
	}
	
	if(current_dimming_value >= 80)
		fault_detect_b_val = temp_here ;
}

void check_fault_detect(void)
{
	if(main_fast_loop_counter%20 == 0)
	{
		if(((fault_detect_a_val < MIN_FAULT_DETECT_VOLT )
		  ||(fault_detect_a_val > MAX_FAULT_DETECT_VOLT ))
		  &&(vbuf_volt > (VBUF_VOLT_CENTER - VBUF_VOLT_OFF_SET)))
	  {
		  if(counter_fault_a_detect <= TIME_OUT_FAULT_DETECT)
			  counter_fault_a_detect ++ ;
	  }
	  else
		  counter_fault_a_detect = 0 ;
	
	  if(((fault_detect_b_val < MIN_FAULT_DETECT_VOLT )
		  ||(fault_detect_b_val > MAX_FAULT_DETECT_VOLT ))
		  &&(vbuf_volt > (VBUF_VOLT_CENTER - VBUF_VOLT_OFF_SET)))
	  {
		  if(counter_fault_b_detect <= TIME_OUT_FAULT_DETECT)
			  counter_fault_b_detect ++ ;
	  }
	  else
		  counter_fault_b_detect = 0 ;
	
	  //if((input_current_rms_100 <= CCR_CUR_DETECT_LEVEL)
			//&&(current_dimming_value <= 4))
		if(current_dimming_value < 4)
	  {
		  counter_fault_b_detect = 0;
		  counter_fault_a_detect = 0;
	  }
	
	  if((counter_fault_a_detect >= TIME_OUT_FAULT_DETECT)
		  ||(counter_fault_b_detect >= TIME_OUT_FAULT_DETECT))
	  {
		  flag_fault_detect = FLAG_LED_IS_FAULT ;
	  }
	}
}	

void update_relay_control(void)
{
	
	if(flag_fault_detect == FLAG_LED_IS_NOT_FAULT)
	{
		clr_relay_open_coil();		
		set_relay_close_coil();			
	}
	
	if(flag_fault_detect == FLAG_LED_IS_FAULT)
	{
		clr_relay_close_coil();
		set_relay_open_coil();
		fuse_open_enable();
	}
}
