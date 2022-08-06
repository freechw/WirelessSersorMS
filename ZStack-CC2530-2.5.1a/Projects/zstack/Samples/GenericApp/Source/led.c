#include"hal_types.h"
#include"led.h"
uchar leddata[4];
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
uchar *Send_LedStatus(void)
{
	leddata[0]='$';
	if(LED1==0)
		leddata[1]='0';
	else
		leddata[1]='1';
	leddata[2]='#';
	leddata[3]='\0';
	return leddata;
}
