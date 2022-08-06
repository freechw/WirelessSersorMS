#include "ioCC2530.h" 
#include "hal_types.h"


#define HONGWAI P0_5 //定义P0.5口为红外控制�?

void Init_infrare(void)
{
	P0SEL &= ~0x20; 
	P0DIR &= ~0x20;  
}
