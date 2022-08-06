#include"relay.h"
#include "GenericApp.h"
#define REL P1_3 
uchar relaydata[4];
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
void RelyCtrl(uint8_t cmd)
{
	switch(cmd)
	{
		case RELY_OFF:
			RelayOff();
			break;
		case RELY_ON:
			RelayOn();			
			break;
		default:
			break;
	}

	return;
}
uchar *sendRelayStatus(void)
{
	relaydata[0]='$';
	if(REL==1)
		relaydata[1]='1';
	else
		relaydata[1]='0';
	relaydata[2]='#';
	relaydata[3]='\0';
	return relaydata;
}