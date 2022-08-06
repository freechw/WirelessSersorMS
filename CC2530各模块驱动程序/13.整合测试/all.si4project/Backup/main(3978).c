/****************************************************************************
* 文 件 名: main.c
* 作    者: Daniel Peng
* 修    订: 2022-4-10
* 版    本: 1.0
* 描    述: 将采集到的温湿度通过串口发送到串口调试助手上显示 115200 8N1
****************************************************************************/
#include <stdio.h>
#include "hal_lcd.h"
#include <ioCC2530.h>
#include <string.h>
#include "hal_types.h"
#include "chinese.h"
#include "UART.H" 
#include"relay.h"
#include"light.h"
#include "DHT11.H" 
#include"relay.h"
#include"gas.h"
#include"hongwai.h"
typedef unsigned int  uint;
typedef unsigned short uint16_t;

char TxData[2];    //存储发送字符串
uint16 LightLevel;




uint16 myApp_ReadLightLevel( void );

/****************************************************************************
* 程序入口函数
****************************************************************************/
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
void SyncData(unsigned char  sto_no)  //把所有采集的数据同步到结构体EnvMsg中
{
	char buf[10]={0};

#if 0
	sprintf(buf,"peo:%c",(HONGWAI==1)?'Y':'N');	
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

#endif
	
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
    CLKCONCMD &= ~0x40;      // 设置系统时钟源为 32MHZ晶振
    while(CLKCONSTA & 0x40); // 等待晶振稳定 
    CLKCONCMD &= ~0x47;      // 设置系统主时钟频率为 32MHZ


    InitBeep();
    
  //  InitUart();              //串口初始化

	Delay_ms(1000);          //让设备稳定

	HalLcdInit();
		
	BACK_COLOR=MAGENTA;
	POINT_COLOR=YELLOW;
	LCD_Clear(BACK_COLOR);	

	
	InitRelay();
	Init_infrare();
	

    while(1)	
    { 
        uchar temp[3]; 
    uchar humidity[3];   
    uchar strTemp[13]="Temperature:";
    uchar strHumidity[10]="Humidity:";
        memset(temp, 0, 3);
        memset(humidity, 0, 3);
   
        DHT11();             //获取温湿度


       // LCD_ShowString(41,40,strHumidity);
      ///  LCD_ShowString(41,60,humidity);
		HalLcdWriteString(strHumidity,HAL_LCD_LINE_8);      
        Delay_ms(2000);  //延时，2S读取1次 
    }
#if 0
    {
		SyncData(1);
		
		Delay_ms(2000);  //延时，2S读取1次 

	}
#endif

}
