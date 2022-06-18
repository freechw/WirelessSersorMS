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


typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned short uint16_t;

extern unsigned char gImage_pic[32776];

char TxData[2];    //�洢�����ַ���
uint16 LightLevel;

extern __code const unsigned char gImage_liang[];
extern __code const unsigned char gImage_mie[];
extern const unsigned char gImage_qq[];


uint16 myApp_ReadLightLevel( void );
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
* ��    ��: InitUart()
* ��    ��: ���ڳ�ʼ������
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/
void InitUart(void)
{ 
    PERCFG = 0x00;           //������ƼĴ��� USART 0��IOλ��:0ΪP0��λ��1 
    P0SEL = 0x0c;            //P0_2,P0_3�������ڣ����蹦�ܣ�
    P2DIR &= ~0XC0;          //P0������ΪUART0
    
    U0CSR |= 0x80;           //����ΪUART��ʽ
    U0GCR |= 11;				       
    U0BAUD |= 216;           //��������Ϊ115200
    UTX0IF = 0;              //UART0 TX�жϱ�־��ʼ��λ0
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
	
    CLKCONCMD &= ~0x40;               //����ϵͳʱ��ԴΪ32MHZ����
    while(CLKCONSTA & 0x40);          //�ȴ������ȶ�Ϊ32M
    CLKCONCMD &= ~0x47;               //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ  

	InitBeep();
	InitUart();                       //���ô�����ؼĴ���
	HalLcdInit();
	
	BACK_COLOR=MAGENTA;//��ʼ������ɫ
	POINT_COLOR=YELLOW;//������ɫΪ��ɫ�������������ɫ��Ҫ���ģ����Ը���hal_lcd.h�������ض�����и��� 
	LCD_Clear(BACK_COLOR); //����

	#if 0
	/*��ʾһ��Linux*/
	showhanzi16x16(0,60,0,gong,YELLOW,BACK_COLOR);
	showhanzi16x16(17,60,0,zhong,YELLOW,BACK_COLOR);
	showhanzi16x16(34,60,0,hao,YELLOW,BACK_COLOR);

    LCD_ShowString(51,60,":");
   
    showhanzi16x16(53,60,0,yi,YELLOW,BACK_COLOR);
    showhanzi16x16(70,60,0,kou,YELLOW,BACK_COLOR);
	LCD_ShowString(88,60,linux);
	#endif

#if 1
	/*��ʾһ��Linux*/
	showhanzi16x16(0,0,0,gong,YELLOW,BACK_COLOR);
	showhanzi16x16(17,0,0,zhong,YELLOW,BACK_COLOR);
	showhanzi16x16(34,0,0,hao,YELLOW,BACK_COLOR);

    LCD_ShowString(51,0,":");
   
    showhanzi16x16(53,0,0,yi,YELLOW,BACK_COLOR);
    showhanzi16x16(70,0,0,kou,YELLOW,BACK_COLOR);
	LCD_ShowString(88,0,linux);
#endif

	

	//yikouDraw_Circle(60,30,10);��Բ
	
	//UartSendString(strTemp,12); //��������

	DelayMS(1000);//��ʱ���������û�˺�������ȡʱ��̫�̣��򴮿�������̫����
	
	//showimage_liang(40,80);
	showimage(40,40,40,40,gImage_qq); //gImage_pic gImage_qq __code

     while(1);

}

/*
          BACK_COLOR=BLUE;//��ʼ������ɫΪ��ɫ
          POINT_COLOR=WHITE;//������ɫΪ��ɫ�������������ɫ��Ҫ���ģ����Ը���hal_lcd.h�������ض�����и��� 
          LCD_Clear(BACK_COLOR); //����
          
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

