#ifndef __ZIGBEE_CMD_H__
#define __ZIGBEE_CMD_H__

#define N 8


#define UINT8 unsigned char 

struct msg
{
	long type;
	long msgtype;
	unsigned char text[N];
};

enum{
	DEVICE_FAN = 0,
	DEVICE_BEEP,
	DEVICE_LED,
	DEVICE_MAX
};

/*	|--|--|----|
	 no  dev  op
	 10   10  0001
	 
*/
static UINT8 born_cmd(UINT8 ep_no,UINT8 device,UINT8 data)
{
	return ((ep_no - 48)) << 6 | (device << 4) | (data << 0);
}



#endif
