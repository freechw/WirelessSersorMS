
#include "ioCC2530.h" 
#include "hal_types.h"
#include"head.h"

#define REL P1_3 

void InitRelay(void)
{
	P1DIR |= 0x08;
	REL = 0;
}
void RelayOn(void)
{
	REL = 0;
}
void RelayOff(void)
{
	REL = 1;
}
void RelyCtrl(uint8 cmd)
{
	switch(cmd)
	{
		case RELY_OFF:

			RelayOn();
			
		
			break;
		case RELY_ON:

			RelayOff();
			
			break;
		default:
			break;
	}

	return;
}


