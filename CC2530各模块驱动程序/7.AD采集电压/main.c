/****************************************************************************
* 文 件 名: main.c
* 作    者: Daniel Peng
* 修    订: 2022-4-10
* 版    本: 1.0
* 描    述: 将 1/3 电压值通过 ADC 转换为数字信号，再将其通过串口发送到 PC
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "UartTimer.h"


//定义控制灯的端口
#define led1 P1_0
#define led2 P1_1
#define BEEP P0_0        //定义P0.0定义为蜂鸣器控制口
/****************************************************************************
* 名    称: InitBeep()
* 功    能: 初始化蜂鸣器，默认关闭
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void InitBeep(void)
{
	P0DIR |= 0x01;    //P0.0定义为输出口 
	BEEP = 0;        //蜂鸣器灭
}
/****************************************************************************
* 名    称: InitSensor()
* 功    能: AD初始化函数
* 入口参数: 无
* 出口参数: 无
****************************************************************************/ 
void InitialAD(void)
{
	P1DIR = 0x03; //P1 控制 LED
	led1 = 1;
	led2 = 1; //关 LED

	ADCH &= 0X00; //清 EOC 标志 ADCCON1.EOC位是一个状态位，当一个转换结束时，设置为高电平；当读取ADCH时，它就被清除。 
	ADCCON3=0xbf; //单次转换,参考电压为电源电压，对 1/3 VDD 进行 A/D 转换
	//14 位分辨率bf
	ADCCON1 = 0X30; //停止 A/D
	ADCCON1 |= 0X40; //启动 A/D
}
/****************************************************************************
* 程序入口函数
****************************************************************************/
void main(void) 
{   
	char temp[2];
	float num;  
    char strTemp[12]={0};
	unsigned short reading;

	InitBeep();
	DISABLE_ALL_INTERRUPTS();     //关闭所有中断 
	InitClock();                  //设置系统主时钟为 32M 
    InitUART();                           //初始化串口 
    InitialAD();
    
	led1 = 1;
#if 0
//12bite  ADCCON3=0xbf  11
    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //转换完毕指示  
		  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 2;
		ADCCON1 |= 0x40; //开始下一转换
		
        num = (float)(reading)*3.3/8192; //有一位符号位,取 2^13;
        //定参考电压为 3.3V。 14 位精确度
 	
		sprintf(strTemp,"vol:%.02fV", num);//将浮点数转成字符串
        UartSendString(strTemp,12); //串口送数
        //包括空格
        DelayMS(1000);
		led1 = 1; //完成数据处理
		DelayMS(1000);
      }
    }  


//7bite  
	ADCCON3=0x8f;

    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //转换完毕指示  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 8;
         ADCCON1 |= 0x40; //开始下一转换
	
		
        num = (float)(reading)*3.3/128; //有一位符号位,取 2^7;
        //定参考电压为 3.3V。 7 位精确度
 	
		sprintf(strTemp,"vol:%.02fV", num);//将浮点数转成字符串
        UartSendString(strTemp,12); //串口送数
        //包括空格
        DelayMS(1000);
		led1 = 1; //完成数据处理
		DelayMS(1000);
      }
    }

 //9bite   
 	ADCCON3=0x9f;

    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //转换完毕指示  
		  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 6;
         ADCCON1 |= 0x40; //开始下一转换
	
		
        num = (float)(reading)*3.3/512; //有一位符号位,取 2^9;
        //定参考电压为 3.3V。 14 位精确度
 	
		sprintf(strTemp,"vol:%.02fV", num);//将浮点数转成字符串
        UartSendString(strTemp,12); //串口送数
        //包括空格
        DelayMS(1000);
		led1 = 1; //完成数据处理
		DelayMS(1000);
      }
    }
#endif
#if 1
//10bite   
	ADCCON3=0xaf;

    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //转换完毕指示  
		  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 5;
         ADCCON1 |= 0x40; //开始下一转换
	
		
        num = (float)(reading)*3.3/1024; //有一位符号位,取 2^10;
        //定参考电压为 3.3V。 14 位精确度
 	
		sprintf(strTemp,"vol:%.02fV", num);//将浮点数转成字符串
        UartSendString(strTemp,12); //串口送数
        //包括空格
        DelayMS(1000);
		led1 = 1; //完成数据处理
		DelayMS(1000);
      }
    }

#endif
#if 0
//12bite  
	ADCCON3=0xbf;
    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //转换完毕指示  
		  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 2;
		ADCCON1 |= 0x40; //开始下一转换
		
        num = (float)(reading)*3.3/8192; //有一位符号位,取 2^13;
        //定参考电压为 3.3V。 14 位精确度
 	
		sprintf(strTemp,"vol:%.02fV", num);//将浮点数转成字符串
        UartSendString(strTemp,12); //串口送数
        //包括空格
        DelayMS(1000);
		led1 = 1; //完成数据处理
		DelayMS(1000);
      }
    }  
#endif
}