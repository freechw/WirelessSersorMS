/****************************************************************************
* 文 件 名: UART.c
* 作    者: Daniel Peng
* 修    订: 2022-5-10
* 版    本: 1.0
* 描    述: UART驱动
****************************************************************************/
#include <ioCC2530.h>
#include "UART.H" 

// 串口初始化函数     
void InitUart()
{
    
    PERCFG = 0x00;           //位置1 P0口 
    P0SEL |= 0x0c;            //P0_2,P0_3,P0_4,P0_5用作串口,第二功能 
    P2DIR &= ~0xC0;          //P0 优先作为UART0 ，优先级
    
    U0CSR |= 0x80;           //UART 方式 
    U0GCR |= 11;             //U0GCR与U0BAUD配合     
    U0BAUD |= 216;           // 波特率设为115200 
    UTX0IF = 0;              //UART0 TX 中断标志初始置位0 
    
}

//串口发送函数    
void UartSendString(unsigned char *Data, int len) 
{
    int j; 
	U0CSR &= ~0x40;                    //绂佹鎺ユ敹
    for(j=0;j<len;j++) 
    { 
        U0DBUF = *Data++; 
        while(UTX0IF == 0); 
        UTX0IF = 0; 
    } 
    U0CSR |= 0x40;
}