/****************************************************************************
* 文 件 名: main.c
* 作    者: Daniel Peng
* 修    订: 2022-5-10
* 版    本: 1.0
* 描    述: 光敏模块驱动
*           设置串口调试助手波特率：115200bps 8N1
*           串口调试助手给CC2530发字符串时，开发板会返回接收到的字符串
****************************************************************************/
#include <stdio.h>

#include "ioCC2530.h" 
#include "string.h"
#include "type.h"

 
typedef unsigned char uchar;
typedef unsigned int  uint;

char TxData[2];    //存储发送字符串
uint16 LightLevel;

uint16 myApp_ReadLightLevel( void );
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
* 名    称: InitUart()
* 功    能: 串口初始化函数
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void InitUart(void)
{ 
    PERCFG = 0x00;           //外设控制寄存器 USART 0的IO位置:0为P0口位置1 
    P0SEL = 0x0c;            //P0_2,P0_3用作串口（外设功能）
    P2DIR &= ~0XC0;          //P0优先作为UART0
    
    U0CSR |= 0x80;           //设置为UART方式
    U0GCR |= 11;				       
    U0BAUD |= 216;           //波特率设为115200
    UTX0IF = 0;              //UART0 TX中断标志初始置位0
}

/****************************************************************************
* 名    称: UartSendString()
* 功    能: 串口发送函数
* 入口参数: Data:发送缓冲区   len:发送长度
* 出口参数: 无
****************************************************************************/
void UartSendString(char *Data, int len)
{
    uint i;
    
    for(i=0; i<len; i++)
    {
        U0DBUF = *Data++;
        while(UTX0IF == 0);
        UTX0IF = 0;
    }
}

/****************************************************************************
* 名    称: DelayMS()
* 功    能: 以毫秒为单位延时 16M时约为535,32M时要调整,系统时钟不修改默认为16M
* 入口参数: msec 延时参数，值越大延时越久
* 出口参数: 无
****************************************************************************/
void DelayMS(uint msec)
{  
    uint i,j;
    
    for (i=0; i<msec; i++)
        for (j=0; j<1070; j++);
}

/******************************************************************************
* @fn  main
*
* @brief
*      Main function of application example.
*
* Parameters:
*
* @param  void
*
* @return void
*
******************************************************************************/
void main(void)
{
	char strTemp[12]={0};
	
    CLKCONCMD &= ~0x40;               //设置系统时钟源为32MHZ晶振
    while(CLKCONSTA & 0x40);          //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;               //设置系统主时钟频率为32MHZ  

	InitBeep();
	InitUart();                       //调置串口相关寄存器

	while(1)
	{
	  LightLevel = myApp_ReadLightLevel();
	  /*上面一条语句是读取光敏电阻引脚上的ad转换值，并没有换算成能表示光强强度的值
	  主要是示意大家如何使用2530芯片的AD功能*/

	  strTemp[0] = LightLevel / 100 + '0';
	  strTemp[1] = LightLevel / 10%10 + '0';
	  strTemp[2] = LightLevel % 10 + '0';
	  strTemp[3] = '\r';
	  strTemp[4] = '\n';

 	 	  
	  UartSendString(strTemp,12); //串口送数
	  
	  DelayMS(2000);//延时函数，如果没此函数，读取时间太短，向串口送数据太快了
	}
}


uint16 myApp_ReadLightLevel( void )
{
  uint16 reading = 0;
  
  /* Enable channel */
  ADCCFG |= 0x40;
  
  /* writing to this register starts the extra conversion */
  ADCCON3 = 0x86;

  ADCCON1 |= 0x40; //开始下一转换 可不加
    
  /* Wait for the conversion to be done */
  while (!(ADCCON1 & 0x80));
  
  /* Disable channel after done conversion */
  ADCCFG &= (0x40 ^ 0xFF);
  
  /* Read the result */
  reading = ADCL;
  reading |= (int16) (ADCH << 8); 
  
  reading >>= 8;

  return (reading);
}
