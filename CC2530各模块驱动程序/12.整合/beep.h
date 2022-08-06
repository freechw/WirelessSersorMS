#ifndef BEEP_H
#define BEEP_H
#include "ioCC2530.h" 
#include "hal_types.h"
extern void InitBeep(void);
extern void BeepOn(void);
extern void BeepOff(void);
extern void BeepCtrl(uint8 cmd);
#endif