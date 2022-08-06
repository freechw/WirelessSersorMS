#ifndef RELAY_H
#define RELAY_H
#include"head.h"
extern void InitRelay(void);
extern void RelayOn(void);
extern void RelayOff(void);
extern void RelyCtrl(unsigned char cmd);
extern uchar *sendRelayStatus(void);
#endif