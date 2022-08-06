#include <ioCC2530.h>
#include"beep.h"
#include"head.h"

#define BEEP_PIN P0_0        //定义P0.0定义为蜂鸣器控制口
/****************************************************************************
* 名    称: InitBeep()
* 功    能: 初始化蜂鸣器，默认关闭
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void InitBeep(void)
{
	P0DIR |= 0x01;     //P0.0定义为输出口 
	BEEP_PIN = 0;        //��������
}
void BeepOn(void)
{
	P0DIR |= 0x01;
	BEEP_PIN = 1;
}
void BeepOff(void)
{
	P0DIR |= 0x01;
	BEEP_PIN = 0;
}
void BeepCtrl(uint8 cmd)
{

	switch(cmd)
	{
		case BEEP_OFF:
			//DeviceStatus &= ~0x02;
			BeepOff();
			//HalLcdWriteString("beep on",HAL_LCD_LINE_5);
			break;
		case BEEP_ON:
			//DeviceStatus |= 0x02;
			BeepOn();
			//HalLcdWriteString("beep off",HAL_LCD_LINE_5);
			break;
		default:
			break;
	}
}

