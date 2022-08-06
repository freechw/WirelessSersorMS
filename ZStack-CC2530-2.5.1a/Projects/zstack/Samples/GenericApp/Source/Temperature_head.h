#ifndef TEMPERATURE_HEAD_H
#define TEMPERATURE_HEAD_H
#include"head.h"
extern void Hal_HW_WaitUs(uint microSecs);
extern void init_1820(void);
extern void write_1820(uint x);
extern uchar read_1820(void);
extern void get_id(void);
extern void read_data(void);
extern void myApp_ReadTemperature( void );
extern uchar *Temperature_readAndsend(void);
#endif