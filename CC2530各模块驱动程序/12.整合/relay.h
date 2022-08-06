#ifndef RELAY_H
#define RELAY_H

#include "ioCC2530.h" 
#include "hal_types.h"

extern void InitRelay(void);
extern void RelayOn(void);
extern void RelayOff(void);
extern void RelyCtrl(unsigned char cmd);

#endif

