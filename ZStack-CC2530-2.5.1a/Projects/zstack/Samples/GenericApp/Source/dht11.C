/****************************************************************************
* 文 件 名: DHT11.c
* 作    者: Daniel Peng
* 修    订: 2022-5-10
* 版    本: 1.0
* 描    述: 温湿度传感器驱动
****************************************************************************/
#include <ioCC2530.h>
#include "dht11.h"

#include "OnBoard.h"


#define LED1 P1_0 
#define LED2 P1_1
#define LED3 P1_4 

#define DATA_PIN P0_4

//温湿度定义
uchar ucharFLAG,uchartemp;
uchar shidu_shi,shidu_ge,wendu_shi,wendu_ge=4;
uchar ucharT_data_H,ucharT_data_L,ucharRH_data_H,ucharRH_data_L,ucharcheckdata;
uchar ucharT_data_H_temp,ucharT_data_L_temp,ucharRH_data_H_temp,ucharRH_data_L_temp,ucharcheckdata_temp;
uchar ucharcomdata;

//延时函数
void Delay_us(void) //1 us延时
{
	MicroWait(1); 
}

void Delay_10us(void) //10 us延时
{
	MicroWait(10);  
}

void Delay_ms(unsigned short Time)//n ms延时
{
    unsigned char i;
    while(Time--)
    {
        for(i=0;i<100;i++)
            Delay_10us();
    }
}
void DATA_IO_input_Cfg(void)//设置为上拉输入模式
{
    P0SEL &= 0xEF;   // 1110 1111 把P12设置为普通IO模式
    P0DIR &= 0xEF;   // 1110 1111 把P12设置为输入模式
    P0INP &= 0xEF;   // 1110 1111 把P12设置为上下拉模式
}

void DATA_IO_output_Cfg(void)//设置为输出模式
{
    P0SEL &= 0xEF;   // 1110 1111 把P12设置为普通IO模式
    P0DIR |= 0x10;   // 0001 0000 把P12设置为输出模式
}
//温湿度传感
void COM(void)    // 温湿写入
{     
    uchar i;         
    for(i=0;i<8;i++)    
    {
        while(!DATA_PIN);
        Delay_10us();
        Delay_10us();
		Delay_10us();
 
        uchartemp=0;
        if(DATA_PIN)
			uchartemp=1;
        while(DATA_PIN);   
        ucharcomdata<<=1;
        ucharcomdata|=uchartemp; 
    }    
}

void readDHT11(void)   //温湿传感启动
{

	LED1 = 1;
	DATA_IO_output_Cfg();
	
    DATA_PIN=0;
    Delay_ms(20);  //>18MS
    DATA_PIN=1; 
   // P1DIR &= ~0x04; //重新配置IO口方向输入
   
    Delay_10us();
    Delay_10us();                        
    Delay_10us();
    Delay_10us();

	DATA_IO_input_Cfg();	   
	
	#if 1
    if(!DATA_PIN) //判断从机是否低电平应答
    {
        while(!DATA_PIN);
        while(DATA_PIN); 
        COM();
        ucharRH_data_H_temp=ucharcomdata;
        COM();
        ucharRH_data_L_temp=ucharcomdata;
        COM();
        ucharT_data_H_temp=ucharcomdata;
        COM();
        ucharT_data_L_temp=ucharcomdata;
        COM();
        ucharcheckdata_temp=ucharcomdata;
        DATA_PIN=1; 
		LED1 = 0;
        uchartemp=(ucharT_data_H_temp+ucharT_data_L_temp+ucharRH_data_H_temp+ucharRH_data_L_temp);
        if(uchartemp==ucharcheckdata_temp)
        {
            ucharRH_data_H=ucharRH_data_H_temp;
            ucharRH_data_L=ucharRH_data_L_temp;
            ucharT_data_H=ucharT_data_H_temp;
            ucharT_data_L=ucharT_data_L_temp;
            ucharcheckdata=ucharcheckdata_temp;
        }
        wendu_shi=ucharT_data_H/10; 
        wendu_ge=ucharT_data_H%10;
        
        shidu_shi=ucharRH_data_H/10; 
        shidu_ge=ucharRH_data_H%10;        
    } 
    else //没用成功读取，返回0
    {
    	LED2 =1 ;
        wendu_shi=0; 
        wendu_ge=0;
        
        shidu_shi=0; 
        shidu_ge=0;  
    } 
	#endif
}
