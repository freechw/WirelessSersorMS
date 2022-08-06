#ifndef LED_H
#define LED_H
#include"head.h"
extern void InitLed(void);
extern void LedOn(void);
extern void LedOff(void);
extern uchar *Send_LedStatus(void);
#endif