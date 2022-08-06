#include "ioCC2530.h" 
#include"hal_types.h"
#include"led.h"
#include"head.h"

uint8 leddata[4];
#define LED1 P1_0 
#define LED2 P1_1
#define LED3 P1_4 
void InitLed(void)
{
	P1DIR |= 0x13; 
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
}
void LedOn(void)
{
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
}
void LedOff(void)
{
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
}
void LedCtrl(uint8 cmd)
{
	switch(cmd)
	{
		case LED_OFF:
			//DeviceStatus &= ~0x04;
			//HalLcdWriteString("led off",HAL_LCD_LINE_5);
			LedOff();
			break;
		case LED_ON:
			//DeviceStatus |= 0x04;
			//HalLcdWriteString("led on",HAL_LCD_LINE_5);
			LedOn();  
			break;
		default:
			break;
	}
}

