#include "hal_types.h"
/**************************************************************************************************
 *                                         CONSTANTS
 **************************************************************************************************/

/* These are used to specify which line the text will be printed */
#define HAL_LCD_LINE_1      0x01
#define HAL_LCD_LINE_2      0x02
/*
   This to support LCD with extended number of lines (more than 2).
   Don't use these if LCD doesn't support more than 2 lines
*/
#define HAL_LCD_LINE_3      0x03
#define HAL_LCD_LINE_4      0x04
#define HAL_LCD_LINE_5      0x05
#define HAL_LCD_LINE_6      0x06
#define HAL_LCD_LINE_7      0x07
#define HAL_LCD_LINE_8      0x08

//����LCD�ĳߴ�	
#define LCD_W 130
#define LCD_H 132

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


extern  unsigned short BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

void HalLcdInit(void); 
void HalLcd_HW_Init(void);

void LCD_Clear(unsigned short Color);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da);
void LCD_WR_DATA(int da);
void LCD_WR_REG(unsigned char da);

void LCD_DrawPoint(unsigned short x,unsigned short y);//����
void LCD_DrawPoint_big(unsigned short x,unsigned short y);//��һ�����
unsigned short  LCD_ReadPoint(unsigned short x,unsigned short y); //����
void Draw_Circle(unsigned short x0,unsigned short y0,unsigned char r);
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);		   
void LCD_Fill(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short color);
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned char mode);//��ʾһ���ַ�
void LCD_ShowNum(unsigned short x,unsigned short y,unsigned int num,unsigned char len);//��ʾ����
void LCD_Show2Num(unsigned short x,unsigned short y,unsigned short num,unsigned char len);//��ʾ2������
void LCD_ShowString(unsigned short x,unsigned short y,const unsigned char *p);		 //��ʾһ���ַ���,16����
void showhanzi24x24(unsigned int x,unsigned int y,unsigned char index, unsigned char *temp, unsigned int pointcolor, unsigned int back_color);
void showhanzi16x16(unsigned int x,unsigned int y,unsigned char index, unsigned char *temp, unsigned int pointcolor, unsigned int back_color);
void HalLcdWriteString(char *str, unsigned char option);
void HalLcdWriteStringValue( char *title, uint16 value, uint8 format, uint8 line );
void HalLcdWriteScreen( char *line1, char *line2 );
void HalLcdWriteStringValueValue( char *title, uint16 value1, uint8 format1,
                                  uint16 value2, uint8 format2, uint8 line );
void HalLcdWriteValue ( uint32 value, const uint8 radix, uint8 option);
void DispPic(const unsigned char *p);