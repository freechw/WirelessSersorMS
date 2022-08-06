#ifndef LED_H
#define LED_H
#include "ioCC2530.h" 
#include "hal_types.h"
extern void InitLed(void);
extern void LedOn(void);
extern void LedOff(void);
extern void LedCtrl(uint8 cmd);
extern uint8 *Send_LedStatus(void);
#endif