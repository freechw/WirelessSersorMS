#include "lpc11xx.h"
#include "gpio.h"
#include "timer32.h"
#include "timer16.h"
#include "led_spk_fan.h"

#define BUZZER // 有源蜂鸣器

#define FANSPEEDPWMCNTMAX		100
uint8_t FanSpeedPwm;
uint8_t bzzerPwm;
extern volatile uint32_t timer16_0_counter;

void InitLed(void)
{
	// LED1
  GPIOSetDir(PORT3, 0, 1);			// Set PIO3_0 to output
  GPIOSetValue(PORT3, 0, 1);		// PIO3_0 output 1, close LED1
	
	// LED2
  GPIOSetDir(PORT3, 1, 1);			// Set PIO3_1 to output
  GPIOSetValue(PORT3, 1, 1);		// PIO1_1 output 1, close LED2
}


void InitFan(void)
{
	// Fan
 	GPIOSetDir(PORT0, 2, 1);			// Set PIO0_2 to output    
  	GPIOSetValue(PORT0, 2, 1);		// PIO0_2 output 1, Turn on FAN
	// 初始化定时器1，100us中断，用于扫描排风扇PWM
	init_timer32(1, 4800); // 100us中断
	NVIC_SetPriority(TIMER_32_1_IRQn, 1);
	FanSpeedPwm = 0;
}

void ScanFanPwm(void)
{
	static uint16_t FanSpeedPwmCnt = 0;
	if(FanSpeedPwmCnt < FANSPEEDPWMCNTMAX) 
		FanSpeedPwmCnt++;
	else 
		FanSpeedPwmCnt = 0;
	if(FanSpeedPwmCnt < FanSpeedPwm)
		GPIOSetValue(PORT0, 2, 0);	// PIO0_2 output 0, close FAN	
	else 
		GPIOSetValue(PORT0, 2, 1);	// PIO0_2 output 1, open FAN	
}

void FanSpeed(uint8_t speed)
{
	switch(speed)
	{
		case 0:
			disable_timer32(1);
			GPIOSetValue(PORT0, 2, 1);     // PIO0_2 output 1, open FAN
			FanSpeedPwm = 0;	
			break;
		case 1:
			enable_timer32(1);
			FanSpeedPwm = 30;	
			break;
		case 2:
			enable_timer32(1);
			FanSpeedPwm = 60;	
			break;
		case 3:
			enable_timer32(1);
			FanSpeedPwm = 100;
			break;
	}
}

void InitSpeaker(void)
{
	// Speaker PASS
  LPC_IOCON->R_PIO1_1 &= ~0x07;
  LPC_IOCON->R_PIO1_1 |= 0x01;		//set PIO1_1 gpio function mode
  GPIOSetDir(PORT1, 1, 1);			// Set PIO1_1 to output
  GPIOSetValue(PORT1, 1, 1);		// PIO1_1 output 1,	Speaker turn off

  init_timer16(1, 4800); // 10us中断
  NVIC_SetPriority(TIMER_16_1_IRQn, 3);
  bzzerPwm =0;
}

void ScanbzzerPwm(void)
{
	static uint16_t bzzerPwmCnt = 0;
	if(bzzerPwmCnt < FANSPEEDPWMCNTMAX) 
		bzzerPwmCnt++;
	else 
		bzzerPwmCnt = 0;
	if(bzzerPwmCnt < bzzerPwm)
		GPIOSetValue(PORT1, 1, 0);	// PIO0_2 output 0, close fan	
	else 
		GPIOSetValue(PORT1, 1, 1);// PIO0_2 output 1, open fan	
}

// 1开0关
void Speaker(uint8_t pwm_set)
{
	switch(pwm_set)
	{
		case 1:
			enable_timer16(1);
			bzzerPwm = 50;	   //大于50时 开蜂鸣器
			break;
		case 0:
			disable_timer16(1);
			GPIOSetValue(PORT1, 1, 1);	// PIO1_1 output 0, Turn off SPK
			break;
		default:
			break;
	}
}

// 1开0关
void Led1Show(uint8_t status)
{
	if(status == 1)
		GPIOSetValue(PORT3, 0, 0);		// PIO3_0 output 0, Turn on LED1
	else 
		GPIOSetValue(PORT3, 0, 1);		// PIO3_0 output 0, Turn off LED1
}

/**** End of File ****/
