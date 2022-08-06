#ifndef HEAD_H
#define HEAD_H
#include <ioCC2530.h>
#define STO_MASK	0xc
#define DEV_MASK	0x30 
#define CMD_MASK	0x0f 

#define RELY		0x00
#define BEEP	0x10 
#define LED		0x20 

#define RELY_OFF			0x00
#define RELY_ON				0x01


#define BEEP_OFF			0x00
#define BEEP_ON				0x01
#define BEEP_AUTO_OFF	0x02
#define BEEP_AUTO_ON	0x03

#define LED_OFF		0x00
#define LED_ON		0x01

#endif

