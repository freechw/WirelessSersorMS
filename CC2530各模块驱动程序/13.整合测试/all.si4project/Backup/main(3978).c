/****************************************************************************
* �� �� ��: main.c
* ��    ��: Daniel Peng
* ��    ��: 2022-4-10
* ��    ��: 1.0
* ��    ��: ���ɼ�������ʪ��ͨ�����ڷ��͵����ڵ�����������ʾ 115200 8N1
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

char TxData[2];    //�洢�����ַ���
uint16 LightLevel;




uint16 myApp_ReadLightLevel( void );

/****************************************************************************
* ������ں���
****************************************************************************/
#define BEEP P0_0        //����P0.0����Ϊ���������ƿ�
/****************************************************************************
* ��    ��: InitBeep()
* ��    ��: ��ʼ����������Ĭ�Ϲر�
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/
void InitBeep(void)
{
	P0DIR |= 0x01;    //P0.0����Ϊ����� 
	BEEP = 0;        //��������
}

/****************************************************************************
* ��    ��: DelayMS()
* ��    ��: �Ժ���Ϊ��λ��ʱ 16MʱԼΪ535,32MʱҪ����,ϵͳʱ�Ӳ��޸�Ĭ��Ϊ16M
* ��ڲ���: msec ��ʱ������ֵԽ����ʱԽ��
* ���ڲ���: ��
****************************************************************************/
void DelayMS(uint msec)
{  
    uint i,j;
    
    for (i=0; i<msec; i++)
        for (j=0; j<1070; j++);
}

void showimage(unsigned int x1,unsigned int y1,unsigned int picL,unsigned int picH,const unsigned char *p)
{
    Address_set(x1,y1,x1+picL-1,y1+picH-1);		//��������
    for(int i=0;i<picL*picH;i++)
     {				
             LCD_WR_DATA8(p[i*2+1]);	 //������ɫ����
             LCD_WR_DATA8(p[i*2]);					
     }	
}
extern uchar ucharT_data_H,ucharT_data_L,ucharRH_data_H,ucharRH_data_L,ucharcheckdata;
void SyncData(unsigned char  sto_no)  //�����вɼ�������ͬ�����ṹ��EnvMsg��
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
    CLKCONCMD &= ~0x40;      // ����ϵͳʱ��ԴΪ 32MHZ����
    while(CLKCONSTA & 0x40); // �ȴ������ȶ� 
    CLKCONCMD &= ~0x47;      // ����ϵͳ��ʱ��Ƶ��Ϊ 32MHZ


    InitBeep();
    
  //  InitUart();              //���ڳ�ʼ��

	Delay_ms(1000);          //���豸�ȶ�

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
   
        DHT11();             //��ȡ��ʪ��


       // LCD_ShowString(41,40,strHumidity);
      ///  LCD_ShowString(41,60,humidity);
		HalLcdWriteString(strHumidity,HAL_LCD_LINE_8);      
        Delay_ms(2000);  //��ʱ��2S��ȡ1�� 
    }
#if 0
    {
		SyncData(1);
		
		Delay_ms(2000);  //��ʱ��2S��ȡ1�� 

	}
#endif

}
