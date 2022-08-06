
#include "ioCC2530.h" 
#include "hal_types.h"
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


