/****************************************************************************
* �� �� ��: main.c
* ��    ��: Daniel Peng
* ��    ��: 2022-5-10
* ��    ��: 1.0
* ��    ��: ����ģ������
*           ���ô��ڵ������ֲ����ʣ�115200bps 8N1
*           ���ڵ������ָ�CC2530���ַ���ʱ��������᷵�ؽ��յ����ַ���
****************************************************************************/
#include <stdio.h>

#include "ioCC2530.h" 
#include "string.h"
#include "type.h"

 
typedef unsigned char uchar;
typedef unsigned int  uint;

char TxData[2];    //�洢�����ַ���
uint16 LightLevel;

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
	
    CLKCONCMD &= ~0x40;               //����ϵͳʱ��ԴΪ32MHZ����
    while(CLKCONSTA & 0x40);          //�ȴ������ȶ�Ϊ32M
    CLKCONCMD &= ~0x47;               //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ  

	InitBeep();
	InitUart();                       //���ô�����ؼĴ���

	while(1)
	{
	  LightLevel = myApp_ReadLightLevel();
	  /*����һ������Ƕ�ȡ�������������ϵ�adת��ֵ����û�л�����ܱ�ʾ��ǿǿ�ȵ�ֵ
	  ��Ҫ��ʾ�������ʹ��2530оƬ��AD����*/

	  strTemp[0] = LightLevel / 100 + '0';
	  strTemp[1] = LightLevel / 10%10 + '0';
	  strTemp[2] = LightLevel % 10 + '0';
	  strTemp[3] = '\r';
	  strTemp[4] = '\n';

 	 	  
	  UartSendString(strTemp,12); //��������
	  
	  DelayMS(2000);//��ʱ���������û�˺�������ȡʱ��̫�̣��򴮿�������̫����
	}
}


uint16 myApp_ReadLightLevel( void )
{
  uint16 reading = 0;
  
  /* Enable channel */
  ADCCFG |= 0x40;
  
  /* writing to this register starts the extra conversion */
  ADCCON3 = 0x86;

  ADCCON1 |= 0x40; //��ʼ��һת�� �ɲ���
    
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
