/****************************************************************************
* �� �� ��: main.c
* ��    ��: Daniel Peng
* ��    ��: 2022-4-10
* ��    ��: 1.0
* ��    ��: �� 1/3 ��ѹֵͨ�� ADC ת��Ϊ�����źţ��ٽ���ͨ�����ڷ��͵� PC
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "UartTimer.h"


//������ƵƵĶ˿�
#define led1 P1_0
#define led2 P1_1
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
* ��    ��: InitSensor()
* ��    ��: AD��ʼ������
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/ 
void InitialAD(void)
{
	P1DIR = 0x03; //P1 ���� LED
	led1 = 1;
	led2 = 1; //�� LED

	ADCH &= 0X00; //�� EOC ��־ ADCCON1.EOCλ��һ��״̬λ����һ��ת������ʱ������Ϊ�ߵ�ƽ������ȡADCHʱ�����ͱ������ 
	ADCCON3=0xbf; //����ת��,�ο���ѹΪ��Դ��ѹ���� 1/3 VDD ���� A/D ת��
	//14 λ�ֱ���bf
	ADCCON1 = 0X30; //ֹͣ A/D
	ADCCON1 |= 0X40; //���� A/D
}
/****************************************************************************
* ������ں���
****************************************************************************/
void main(void) 
{   
	char temp[2];
	float num;  
    char strTemp[12]={0};
	unsigned short reading;

	InitBeep();
	DISABLE_ALL_INTERRUPTS();     //�ر������ж� 
	InitClock();                  //����ϵͳ��ʱ��Ϊ 32M 
    InitUART();                           //��ʼ������ 
    InitialAD();
    
	led1 = 1;
#if 0
//12bite  ADCCON3=0xbf  11
    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //ת�����ָʾ  
		  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 2;
		ADCCON1 |= 0x40; //��ʼ��һת��
		
        num = (float)(reading)*3.3/8192; //��һλ����λ,ȡ 2^13;
        //���ο���ѹΪ 3.3V�� 14 λ��ȷ��
 	
		sprintf(strTemp,"vol:%.02fV", num);//��������ת���ַ���
        UartSendString(strTemp,12); //��������
        //�����ո�
        DelayMS(1000);
		led1 = 1; //������ݴ���
		DelayMS(1000);
      }
    }  


//7bite  
	ADCCON3=0x8f;

    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //ת�����ָʾ  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 8;
         ADCCON1 |= 0x40; //��ʼ��һת��
	
		
        num = (float)(reading)*3.3/128; //��һλ����λ,ȡ 2^7;
        //���ο���ѹΪ 3.3V�� 7 λ��ȷ��
 	
		sprintf(strTemp,"vol:%.02fV", num);//��������ת���ַ���
        UartSendString(strTemp,12); //��������
        //�����ո�
        DelayMS(1000);
		led1 = 1; //������ݴ���
		DelayMS(1000);
      }
    }

 //9bite   
 	ADCCON3=0x9f;

    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //ת�����ָʾ  
		  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 6;
         ADCCON1 |= 0x40; //��ʼ��һת��
	
		
        num = (float)(reading)*3.3/512; //��һλ����λ,ȡ 2^9;
        //���ο���ѹΪ 3.3V�� 14 λ��ȷ��
 	
		sprintf(strTemp,"vol:%.02fV", num);//��������ת���ַ���
        UartSendString(strTemp,12); //��������
        //�����ո�
        DelayMS(1000);
		led1 = 1; //������ݴ���
		DelayMS(1000);
      }
    }
#endif
#if 1
//10bite   
	ADCCON3=0xaf;

    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //ת�����ָʾ  
		  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 5;
         ADCCON1 |= 0x40; //��ʼ��һת��
	
		
        num = (float)(reading)*3.3/1024; //��һλ����λ,ȡ 2^10;
        //���ο���ѹΪ 3.3V�� 14 λ��ȷ��
 	
		sprintf(strTemp,"vol:%.02fV", num);//��������ת���ַ���
        UartSendString(strTemp,12); //��������
        //�����ո�
        DelayMS(1000);
		led1 = 1; //������ݴ���
		DelayMS(1000);
      }
    }

#endif
#if 0
//12bite  
	ADCCON3=0xbf;
    while(1)
    {
      if(ADCCON1>=0x80)
      {
		led1 = 0; //ת�����ָʾ  
		  
 		 reading = ADCL;
  		 reading |= ADCH << 8; 
  		 reading >>= 2;
		ADCCON1 |= 0x40; //��ʼ��һת��
		
        num = (float)(reading)*3.3/8192; //��һλ����λ,ȡ 2^13;
        //���ο���ѹΪ 3.3V�� 14 λ��ȷ��
 	
		sprintf(strTemp,"vol:%.02fV", num);//��������ת���ַ���
        UartSendString(strTemp,12); //��������
        //�����ո�
        DelayMS(1000);
		led1 = 1; //������ݴ���
		DelayMS(1000);
      }
    }  
#endif
}