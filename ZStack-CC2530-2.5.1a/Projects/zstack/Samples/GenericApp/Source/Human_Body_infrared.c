#include"hal_types.h"
#include"Infrare_head.h"
uchar Not_detected[30] = "$no#\0";
uchar Detected[30] = "$yes#\0";

#define HONGWAI P0_5 //定义P0.5口为红外控制端

void Init_infrare(void)
{
  P0DIR &= ~0x20;  
  P1DIR |= 0x12;  
  
    P1_1 = 0;
    P1_4 = 0;
    P0_1 = 0;
}
uchar *Infrare_checkandsend(void)
{    
    if(HONGWAI == 1)
     return Detected;
    else
     return Not_detected; 
}
