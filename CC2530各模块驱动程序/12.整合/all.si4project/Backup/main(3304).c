/****************************************************************************
* 文 件 名: main.c
* 作    者: Daniel Peng
* 修    订: 2022-5-10
* 版    本: 1.0
* 描    述: lcd测试程序
****************************************************************************/
#include <stdio.h>
#include "hal_lcd.h"
#include "ioCC2530.h" 
#include "string.h"
#include "hal_types.h"
#include "chinese.h"

#include"relay.h"
#include"light.h"
#include"dht11.h"
#include"relay.h"
#include"gas.h"
#include"hongwai.h"
#include"beep.h"
#include"led.h"
#include"head.h"


typedef unsigned int  uint;
typedef unsigned short uint16_t;

extern unsigned char gImage_pic[32776];

char TxData[2];    //存储发送字符串
uint16 LightLevel;

extern __code const unsigned char gImage_liang[];
extern __code const unsigned char gImage_mie[];
extern const unsigned char gImage_qq[];



uint16 myApp_ReadLightLevel( void );


/****************************************************************************
* 名    称: InitUart()
* 功    能: 串口初始化函数
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void InitUart(void)
{ 
    PERCFG = 0x00;           //外设控制寄存器 USART 0的IO位置:0为P0口位置1 
    P0SEL |= 0x0c;            //P0_2,P0_3用作串口（外设功能）
    P2DIR &= ~0xC0;          //P0优先作为UART0
    
    U0CSR |= 0x80;           //设置为UART方式
    U0GCR |= 11;				       
    U0BAUD |= 216;           //波特率设为115200
    UTX0IF = 0;              //UART0 TX中断标志初始置位0
}

char RxBuf;

void cmd_process(unsigned char cmd)
{
	switch(cmd & DEV_MASK) // 判断设备号
	{
		case RELY: // 排风扇命令字
				// 风扇命令处理函数
			RelyCtrl(cmd & CMD_MASK);
			//HalLcdWriteString("rely",HAL_LCD_LINE_5);
			break;
		case BEEP: // 蜂鸣器命令字
				// 蜂鸣器命令处理函数
			BeepCtrl(cmd & CMD_MASK);
			break;
		case LED: // 照明灯命令字
			// 照明灯命令处理函数
			LedCtrl(cmd & CMD_MASK);
			break;
		default :
			break;
	}

}
/****************************************************************************
* 名    称: UART0_ISR(void) 串口中断处理函数 
* 描    述: 当串口0产生接收中断，将收到的数据保存在RxBuf中
****************************************************************************/
#pragma vector = URX0_VECTOR 
__interrupt void UART0_ISR(void) 
{ 
    URX0IF = 0;       // 清中断标志 
    RxBuf = U0DBUF;  

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
//显示40x40图像，(图像左下角)起点横坐标x，起点纵坐标y，
void showimage_liang(unsigned char x,unsigned char y) //显示40*40图片
{
        Address_set(x,y,x+39,y+39);		//坐标设置
        for(int i=0;i<1600;i++)
             {				
                     LCD_WR_DATA8(gImage_liang[i*2+1]);	 //发送颜色数据
                     LCD_WR_DATA8(gImage_liang[i*2]);					
             }	
}
void showimage(unsigned int x1,unsigned int y1,unsigned int picL,unsigned int picH,const unsigned char *p)
{
    Address_set(x1,y1,x1+picL-1,y1+picH-1);		//坐标设置
    for(int i=0;i<picL*picH;i++)
     {				
             LCD_WR_DATA8(p[i*2+1]);	 //发送颜色数据
             LCD_WR_DATA8(p[i*2]);					
     }	
}
extern uchar ucharT_data_H,ucharT_data_L,ucharRH_data_H,ucharRH_data_L,ucharcheckdata;
void SyncData(unsigned char  sto_no)  
{
	char buf[10]={0};

	sprintf(buf,"people:%c",(HONGWAI==1)?'Y':'N');	
	HalLcdWriteString(buf,HAL_LCD_LINE_4);
	
	DHT11();
	sprintf(buf,"temp:%d",(unsigned char)ucharT_data_H);
	HalLcdWriteString(buf,HAL_LCD_LINE_5);	

	sprintf(buf,"hum:%d",(unsigned char)ucharRH_data_H);
	HalLcdWriteString(buf,HAL_LCD_LINE_6);	

	sprintf(buf,"li:%d",(unsigned int)myApp_ReadLightLevel());

	HalLcdWriteString(buf,HAL_LCD_LINE_7);
	
	sprintf(buf,"gas:%d",(unsigned int)myApp_ReadGasLevel());

	HalLcdWriteString(buf,HAL_LCD_LINE_8);


	
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
#define UART0_RX    1
#define UART0_TX    2
#define SIZE       51

char UartState;
uchar count;
char RxData[SIZE];        //存储发送字符串

#if 0
void main(void)
{	
	InitBeep();
	
    CLKCONCMD &= ~0x40;                        //设置系统时钟源为32MHZ晶振
    while(CLKCONSTA & 0x40);                   //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;                        //设置系统主时钟频率为32MHZ   
   

	
    InitUart();                                //调用串口初始化函数   
    UartState = UART0_RX;                      //串口0默认处于接收模式
    memset(RxData, 0, SIZE);
    
    while(1)
    {
        if(UartState == UART0_RX)              //接收状态 
        { 
            if(RxBuf != 0) 
            {   
            	BeepOn();
                if((RxBuf != '#')&&(count < 50))//以'＃'为结束符,一次最多接收50个字符            
                    RxData[count++] = RxBuf; 
                else
                {
                    if(count >= 50)             //判断数据合法性，防止溢出
                    {
                        count = 0;              //计数清0
                        memset(RxData, 0, SIZE);//清空接收缓冲区
                    }
                    else
                        UartState = UART0_TX;  //进入发送状态 
                }
                RxBuf  = 0;
            }
        }
        
        if(UartState == UART0_TX)              //发送状态 
        {         
        	BeepOff();
            U0CSR &= ~0x40;                    //禁止接收 
            UartSendString(RxData, count);     //发送已记录的字符串。
            U0CSR |= 0x40;                     //允许接收 
            UartState = UART0_RX;              //恢复到接收状态 
            count = 0;                         //计数清0
            memset(RxData, 0, SIZE);           //清空接收缓冲区
        }    
    }
}
#endif
void main()
{
	char buf[10]={0};
	
    CLKCONCMD &= ~0x40;               //设置系统时钟源为32MHZ晶振
    while(CLKCONSTA & 0x40);          //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;               //设置系统主时钟频率为32MHZ  


	InitBeep();
	Delay_ms(1000);
	InitUart(); 
	
	InitRelay();
    InitLed();	    
	Init_infrare();

#if 1
	HalLcdInit();
	
	BACK_COLOR=MAGENTA;
	POINT_COLOR=YELLOW;
	LCD_Clear(BACK_COLOR);

	

	char offset = 20;
	showhanzi16x16(0 + offset,10,0,wu,YELLOW,BACK_COLOR);
	showhanzi16x16(17+ offset,10,0,xian,YELLOW,BACK_COLOR);
	showhanzi16x16(34+ offset,10,0,chuan,YELLOW,BACK_COLOR);
   
    showhanzi16x16(51+ offset,10,0,gan,YELLOW,BACK_COLOR);
    showhanzi16x16(68+ offset,10,0,wang,YELLOW,BACK_COLOR);
#endif
	while(1)
	{
	/*
		sprintf(buf,"%x",RxBuf);
        UartSendString(buf, 4);	
		if(RxBuf != 0) 
        {       	
			cmd_process(RxBuf);
            RxBuf  = 0;
        }*/
		SyncData(1);
		//showimage(85,85,40,40,gImage_qq); //gImage_pic gImage_qq __code
		DelayMS(2000);

	}
	

}

