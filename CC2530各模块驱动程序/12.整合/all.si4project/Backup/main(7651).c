/****************************************************************************
* �� �� ��: main.c
* ��    ��: Daniel Peng
* ��    ��: 2022-5-10
* ��    ��: 1.0
* ��    ��: lcd���Գ���
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
#include"beap.h"


typedef unsigned int  uint;
typedef unsigned short uint16_t;

extern unsigned char gImage_pic[32776];

char TxData[2];    //�洢�����ַ���
uint16 LightLevel;

extern __code const unsigned char gImage_liang[];
extern __code const unsigned char gImage_mie[];
extern const unsigned char gImage_qq[];



uint16 myApp_ReadLightLevel( void );


/****************************************************************************
* ��    ��: InitUart()
* ��    ��: ���ڳ�ʼ������
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/
void InitUart(void)
{ 
    PERCFG = 0x00;           //������ƼĴ��� USART 0��IOλ��:0ΪP0��λ��1 
    P0SEL |= 0x0c;            //P0_2,P0_3�������ڣ����蹦�ܣ�
    P2DIR &= ~0XC0;          //P0������ΪUART0
    
    U0CSR |= 0x80;           //����ΪUART��ʽ
    U0GCR |= 11;				       
    U0BAUD |= 216;           //��������Ϊ115200
    UTX0IF = 0;              //UART0 TX�жϱ�־��ʼ��λ0
}
/****************************************************************************
* ��    ��: UART0_ISR(void) �����жϴ����� 
* ��    ��: ������0���������жϣ����յ������ݱ�����RxBuf��
****************************************************************************/
char RxBuf;

#define STO_MASK	0xc
#define DEV_MASK	0x30 
#define CMD_MASK	0x0f 

#define RELY		0x00
#define BEEP	0x10 
#define LED		0x20 

#define RELY_OFF			0x00
#define RELY_ON				0x01


#define BEEP_OFF			0x00
#define BEEP_ON				0x01
#define BEEP_AUTO_OFF	0x02
#define BEEP_AUTO_ON	0x03

#define LED_OFF		0x00
#define LED_ON		0x01


void cmd_process(unsigned char cmd)
{
	switch(cmd & DEV_MASK) // �ж��豸��
	{
		case RELY: // �ŷ���������
				// �����������
			RelyCtrl(cmd & CMD_MASK);
			//HalLcdWriteString("rely",HAL_LCD_LINE_5);
			break;
		case BEEP: // ������������
				// �������������
			BeepCtrl(cmd & CMD_MASK);
			break;
		case LED: // ������������
			// �������������
			LedCtrl(cmd & CMD_MASK);
			break;
		default :
			break;
	}

}

#pragma vector = URX0_VECTOR 
__interrupt void UART0_ISR(void) 
{ 
    URX0IF = 0;       // ���жϱ�־ 
    RxBuf = U0DBUF;  

	
}


/****************************************************************************
* ��    ��: UartSendString()
* ��    ��: ���ڷ��ͺ���
* ��ڲ���: Data:���ͻ�����   len:���ͳ���
* ���ڲ���: ��
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
//��ʾ40x40ͼ��(ͼ�����½�)��������x�����������y��
void showimage_liang(unsigned char x,unsigned char y) //��ʾ40*40ͼƬ
{
        Address_set(x,y,x+39,y+39);		//��������
        for(int i=0;i<1600;i++)
             {				
                     LCD_WR_DATA8(gImage_liang[i*2+1]);	 //������ɫ����
                     LCD_WR_DATA8(gImage_liang[i*2]);					
             }	
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
void main(void)
{
    CLKCONCMD &= ~0x40;               //����ϵͳʱ��ԴΪ32MHZ����
    while(CLKCONSTA & 0x40);          //�ȴ������ȶ�Ϊ32M
    CLKCONCMD &= ~0x47;               //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ  


	InitBeep();
	Delay_ms(1000);
	InitUart(); 
	
	InitRelay();
    InitLed();	    
	Init_infrare();
	HalLcdInit();
	
	BACK_COLOR=MAGENTA;
	POINT_COLOR=YELLOW;
	LCD_Clear(BACK_COLOR);

	//showimage(40,0,40,40,gImage_qq); //gImage_pic gImage_qq __code

	char offset = 20;
	showhanzi16x16(0 + offset,10,0,wu,YELLOW,BACK_COLOR);
	showhanzi16x16(17+ offset,10,0,xian,YELLOW,BACK_COLOR);
	showhanzi16x16(34+ offset,10,0,chuan,YELLOW,BACK_COLOR);
   
    showhanzi16x16(51+ offset,10,0,gan,YELLOW,BACK_COLOR);
    showhanzi16x16(68+ offset,10,0,wang,YELLOW,BACK_COLOR);

	while(1)
	{
		SyncData(1);
		
		DelayMS(2000);

	}
	

}

