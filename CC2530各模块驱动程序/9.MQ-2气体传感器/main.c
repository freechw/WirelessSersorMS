/******************************************************************************
烟雾传感器，使用P07。

******************************************************************************/
#include "ioCC2530.h" 
#include "string.h"
#include "type.h"


//typedef signed   short  int16;
//typedef unsigned short  uint16;
typedef unsigned char uchar;
typedef unsigned int  uint;

char TxData[5];    //存储发送字符串
uint16 GasLevel;

uint16 myApp_ReadGasLevel( void );
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
    CLKCONCMD &= ~0x40;               //设置系统时钟源为32MHZ晶振
    while(CLKCONSTA & 0x40);          //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;               //设置系统主时钟频率为32MHZ  
  InitBeep();
  InitUart();                       //调置串口相关寄存器

  
  while(1)
  {
      GasLevel = myApp_ReadGasLevel();
      /*上面一条语句是读取烟雾传感器引脚上的ad转换值，并没有换算成能表示烟雾浓度的值
      主要是示意大家如何使用2530芯片的AD功能*/
      
      TxData[0] = GasLevel / 100 + '0';
      TxData[1] = GasLevel / 10%10 + '0';
      TxData[2] = GasLevel % 10 + '0';
	  TxData[3] = '\r';
      TxData[4] = '\n';
      /*以上三句，是吧读取到的数值转换成字符，供串口函数使用*/
      
      UartSendString(TxData, 5);//想串口助手送出数据，波特率是115200

      
      DelayMS(1000);//延时函数，如果没此函数，读取时间太短，向串口送数据太快了
  }
}


uint16 myApp_ReadGasLevel( void )
{
  uint16 reading = 0;
  
  /* Enable channel */
  ADCCFG |= 0x80;
  
  /* writing to this register starts the extra conversion */
  ADCCON3 = 0x87;
  
  /* Wait for the conversion to be done */
  while (!(ADCCON1 & 0x80));
  
  /* Disable channel after done conversion */
  ADCCFG &= (0x80 ^ 0xFF);
  
  /* Read the result */
  reading = ADCL;
  reading |= (int16) (ADCH << 8); 
  
  reading >>= 8;
  
  return (reading);
}