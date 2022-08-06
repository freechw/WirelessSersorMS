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

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned short uint16_t;

extern unsigned char gImage_pic[32776];

char TxData[2];    //存储发送字符串
uint16 LightLevel;

extern __code const unsigned char gImage_liang[];
extern __code const unsigned char gImage_mie[];
extern const unsigned char gImage_qq[];


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

	char linux[]="Linux";
	
    CLKCONCMD &= ~0x40;               //设置系统时钟源为32MHZ晶振
    while(CLKCONSTA & 0x40);          //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;               //设置系统主时钟频率为32MHZ  

	InitBeep();
	InitUart();                       //调置串口相关寄存器
	HalLcdInit();
	
	BACK_COLOR=MAGENTA;//初始化背景色
	POINT_COLOR=YELLOW;//字体颜色为白色，如果这两种颜色需要换的，可以根据hal_lcd.h里面的相关定义进行更换 
	LCD_Clear(BACK_COLOR); //清屏

	#if 0
	/*显示无线传感网*/
	char offset = 20;
	showhanzi16x16(0 + offset,10,0,wu,YELLOW,BACK_COLOR);
	showhanzi16x16(17+ offset,10,0,xian,YELLOW,BACK_COLOR);
	showhanzi16x16(34+ offset,10,0,chuan,YELLOW,BACK_COLOR);
   
    showhanzi16x16(51+ offset,10,0,gan,YELLOW,BACK_COLOR);
    showhanzi16x16(68+ offset,10,0,wang,YELLOW,BACK_COLOR);
	#endif

#if 1
      char offset = 10;
	/*显示一口Linux*/
	showhanzi16x16(0,0 + offset,0,gong,YELLOW,BACK_COLOR);
	showhanzi16x16(17,0 + offset,0,zhong,YELLOW,BACK_COLOR);
	showhanzi16x16(34,0 + offset,0,hao,YELLOW,BACK_COLOR);

    LCD_ShowString(51,0 + offset,":");
   
    showhanzi16x16(53,0 + offset,0,yi,YELLOW,BACK_COLOR);
    showhanzi16x16(70,0 + offset,0,kou,YELLOW,BACK_COLOR);
	LCD_ShowString(88,0 + offset,linux);
#endif

	LCD_DrawLine(0, 34, 128, 34);

	LCD_DrawRectangle(10,80,80,120);


	Draw_Circle(90,60,10);//画圆
	
	//UartSendString(strTemp,12); //串口送数

	DelayMS(1000);//延时函数，如果没此函数，读取时间太短，向串口送数据太快了
	
	//showimage_liang(40,80);
	showimage(40,40,40,40,gImage_qq); //gImage_pic gImage_qq __code
	
     while(1);

}

/*
          BACK_COLOR=BLUE;//初始化背景色为蓝色
          POINT_COLOR=WHITE;//字体颜色为白色，如果这两种颜色需要换的，可以根据hal_lcd.h里面的相关定义进行更换 
          LCD_Clear(BACK_COLOR); //清屏
          
        showhanzi16x16(0,0,0,wen1,WHITE,BACK_COLOR);
        showhanzi16x16(17,0,0,du1,WHITE,BACK_COLOR);
        LCD_ShowString(33,0,":");
        
        showhanzi16x16(0,20,0,shi1,WHITE,BACK_COLOR);
        showhanzi16x16(17,20,0,du1,WHITE,BACK_COLOR);
        LCD_ShowString(33,20,":");
        
        showhanzi16x16(0,40,0,tu,WHITE,BACK_COLOR);
        showhanzi16x16(17,40,0,shi1,WHITE,BACK_COLOR);
        LCD_ShowString(33,40,":");
        
        showhanzi16x16(0,60,0,guang,WHITE,BACK_COLOR);
        showhanzi16x16(17,60,0,qiang,WHITE,BACK_COLOR);
        LCD_ShowString(33,60,":");



        LCD_ShowString(40,0,wendu);
        LCD_ShowString(40,20,shidu);
        LCD_ShowString(40,40,tushi);
        LCD_ShowString(40,60,guangqiang);
		
          showhanzi16x16(0,80,0,zheng,WHITE,BACK_COLOR);
          showhanzi16x16(17,80,0,chang,WHITE,BACK_COLOR);

          showhanzi16x16(0,80,0,bao,WHITE,RED);
          showhanzi16x16(17,80,0,jing,WHITE,RED);
*/

