/****************************************************************************
* �� �� ��: main.c
* ��    ��: Daniel Peng
* ��    ��: 2022-4-10
* ��    ��: 1.0
* ��    ��: ���ɼ�������ʪ��ͨ�����ڷ��͵����ڵ�����������ʾ 115200 8N1
****************************************************************************/
#include <ioCC2530.h>
#include <string.h>
#include "UART.H" 
#include "DHT11.H" 

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
void main(void)
{  
    uchar temp[3]; 
    uchar humidity[3];   
    uchar strTemp[13]="Temperature:";
    uchar strHumidity[10]="Humidity:";
  
    InitBeep();
    Delay_ms(1000);          //���豸�ȶ�
    InitUart();              //���ڳ�ʼ��

    while(1)
    {         
        memset(temp, 0, 3);
        memset(humidity, 0, 3);
   
        DHT11();             //��ȡ��ʪ��

        //����ʪ�ȵ�ת�����ַ���
        temp[0]=wendu_shi+0x30;
        temp[1]=wendu_ge+0x30;
        humidity[0]=shidu_shi+0x30;
        humidity[1]=shidu_ge+0x30;
        
        //��õ���ʪ��ͨ�����������������ʾ
        UartSendString(strTemp, 12);
        UartSendString(temp, 2);
        UartSendString("   ", 3);
        UartSendString(strHumidity, 9);
        UartSendString(humidity, 2);
        UartSendString("\n", 1);
        //LCD_ShowString(41,40,strHumidity);
        //LCD_ShowString(41,60,humidity);
        Delay_ms(2000);  //��ʱ��2S��ȡ1�� 
    }
}
