 
#include "ALL_Includes.h" 

//#include "dimming_curve.h"

/**********************
	Timer1 ,20KHz intteruption for adc RMS calculation	
	
	timer3 ,TIM3_CH1，12PIN , PA6 , 200HZ

	timer14,TIM14_CH1，10PIN , PA4 , 62.5KHZ

***********************/
#define DEBUG_MODE 0 
#define TEST_MODE 1
#define PRODUCTION_MODE 2

void system_init(void);
static void SetSysClock(void);

unsigned char fault_detect_a = 0;
extern unsigned int vbuf_volt ;
unsigned long input_current_rms_100 ;
unsigned int vref_cal_int = 0 ;
//unsigned char flag_relay_status = FLAG_RELAY_IS_NOT_CLOSED ;
extern unsigned char boost_pwm_value ;
unsigned int main_loop_counter = 0 ; 
unsigned int main_fast_loop_counter = 0 ; 
//extern unsigned int target_dimming_value = 0 ; 
extern unsigned int current_dimming_value  ;
extern unsigned int vcc_mcu_real_volt  ;
extern unsigned int mcu_temperature_val ;
extern unsigned int fault_detect_a_val ;
extern unsigned int fault_detect_b_val ;
unsigned char working_mode  ;
extern unsigned char flag_fault_detect ;
extern unsigned int counter_fault_a_detect ;
extern unsigned int counter_fault_b_detect  ;
extern unsigned char first_time_relay_control ;

unsigned char start_up_counter = 0;

//unsigned int vcc_mcu_real_volt_arr[128]  ;


void delay_1uS(void)
{
	unsigned int delay_counter = 10000 ;
	while(delay_counter --)
		LED_ON();
}

void delay_1mS(void)
{
	unsigned int delay_counter = 10000 ;
	while(delay_counter --)
	{
		void delay_1uS();
	}
}

void delay_1S(void)
{
	unsigned int delay_counter = 10000 ;
	while(delay_counter --)
	{
		void delay_1mS();
	}
}

void delay_1H(void)
{
	unsigned int delay_counter = 10000 ;
	while(delay_counter --)
	{
		void delay_1S();
	}
}

void send_data_to_serial_port(unsigned char send_time , unsigned int data_to_send)
{
	unsigned char temp_char ;
	if(send_time == 1)
	{
		temp_char = data_to_send ;
		USART_SendData(USART1,temp_char);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){} 
	}
	if(send_time == 2)
	{
		  temp_char = data_to_send/256 ;
			USART_SendData(USART1,temp_char);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){} 
			temp_char = data_to_send%256 ;
			USART_SendData(USART1,temp_char);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){} 
	}
}

void IWDG_Init(void)
{
IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
IWDG_SetPrescaler(IWDG_Prescaler_256);   
IWDG_SetReload(5000);    
IWDG_ReloadCounter();    
IWDG_Enable();
//printf("WDT RESET");
//printf("\r\n ");
}


int main(void)
{		
	system_init();
	
	boost_pwm_value = INIT_PWM_BOOST ;
	boost_pwm_control(boost_pwm_value);	
	//working_mode = DEBUG_MODE ;
	working_mode = PRODUCTION_MODE ;
	//working_mode = TEST_MODE ;
	set_relay_close_coil();
	
	while(1)
	{
		check_fault_detect();	
		update_relay_control();
		find_dimming_val();
		update_led_dimming_control();
		update_boost_PWM_control();
		update_stability_res_control();
		update_vcc_volt_measure();
		update_mcu_temperature_val();
		update_fault_detect_a_volt();
		update_fault_detect_b_volt();	
		
		//if(working_mode == PRODUCTION_MODE)
		//{
			
		//}
		
		
		if(main_loop_counter % 400 == 0)
		{
			unsigned int temp_int1 ;
			//unsigned char temp_char;
			/*
			temp_int1 = input_current_rms_100 ;
			printf("CCR CURRENT:");
			printf("%d\r\n ", temp_int1);
			
			printf("VBUF VOLT :");
			printf("%d ", vbuf_volt);
			
			printf("FDA : ");
			printf("%d ", fault_detect_a_val);
			//printf("%d ", counter_fault_a_detect);
				
			printf("FDB : ");
			printf("%d\r\n ", fault_detect_b_val);
			//printf("%d\r\n ", counter_fault_b_detect);
			*/
			if(working_mode == DEBUG_MODE)
			{
				printf("VBUF VOLT :");
			  printf("%d ", vbuf_volt);
				
				printf("MCU_VCC_VOLT : ");
				printf("%d ", vcc_mcu_real_volt);
				
				temp_int1 = input_current_rms_100 ;
				
				printf("CCR CURRENT:");
			  printf("%d ", temp_int1);
				
				printf("DIMMING VAL:");
				printf("%d ", current_dimming_value);
				
				printf("TEMP : ");
				printf("%d ", mcu_temperature_val);
				
				printf("FDA : ");
				printf("%d ", fault_detect_a_val);
				printf("%d ", counter_fault_a_detect);
				
				printf("FDB : ");
				printf("%d ", fault_detect_b_val);
				printf("%d ", counter_fault_b_detect);
				
				printf("BOOST PWM DUTY:");
			  printf("%d\r\n ", boost_pwm_value);
				
				printf("TD_RESULT:");
				printf("%d ", flag_fault_detect);
				
			}
		}
		
		
		//delay_us(500);
		IWDG_ReloadCounter();    
		if(main_fast_loop_counter <10000)
			main_fast_loop_counter ++ ;
		
		if(main_fast_loop_counter >=10000)
		{
			//printf("BOOST PWM DUTY:");
			
			//printf("FDA : ");
			//printf("%d\r\n ", fault_detect_a_val);
			//printf("FDB : ");
			//printf("%d\r\n ", fault_detect_b_val);
			main_fast_loop_counter = 0 ;
		}
			
		
		if((main_loop_counter <1000)
			&&(main_fast_loop_counter % 20 == 0))
			main_loop_counter ++ ;
		
		if(main_loop_counter >=1000)
		{
			main_loop_counter = 0 ;
			start_up_counter ++;
		}
		if(start_up_counter>=5)
			start_up_counter = 5;
	}
}

/************************
all initialize
************************/
void system_init(void)
{	
	SetSysClock();
	Delay_Init(88);
	LTC_IO_Init();
	USART1_Init(9600) ;
	TIM1_Config();						//timer1 initialize , 20KHZ intterpution  定时器1 初始化,20KHz	
	TIM3_PWM_Config();				//timer3 initialize PWM , TIM3_CH1 , 12PIN , PA6 , 3.5KHZ
	TIM14_PWM_Config();				//定时器14 PWM初始化,TIM14_CH1，10脚，PA4,62.5KHz
	dimming_control(0);
	ADC1_CH_DMA_Config();				//ADC的DMA通道初始化
	IWDG_Init(); // WDT init
  dimming_control(0);
}


static void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  // SYSCLK, HCLK, PCLK configuration 
  // Enable HSE     
  RCC->CR &=~ ((uint32_t)RCC_CR_HSEON);



  //if (HSEStatus == (uint32_t)0x01)
  {
    // Enable Prefetch Buffer 
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY;

    // HCLK = SYSCLK 
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    // PCLK = HCLK 
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

    //  PLL configuration:  = HSE *  6 = 48 MHz
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)( RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL12);
    RCC_MCOConfig(RCC_MCOSource_HSI);        
    // Enable PLL 
    RCC->CR |= RCC_CR_PLLON;

    // Wait till PLL is ready 
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    // Select PLL as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    // Wait till PLL is used as system clock source 
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  } 
}



