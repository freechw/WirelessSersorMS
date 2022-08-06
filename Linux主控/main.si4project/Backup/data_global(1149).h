#ifndef __DATA_GLOBAL__H__
#define __DATA_GLOBAL__H__

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <syscall.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include "uart_cache.h"


#define DEV_GPRS 			"/dev/ttyUSB0"
#define DEV_ZIGBEE 			"/dev/ttyUSB1"
#define DEV_LED				"/dev/led"
#define DEV_BUZZER			"/dev/beep"
#define DEV_INFRARED		"/dev/key"
#define DEV_CAMERA			"/dev/video0"
#define SQLITE_OPEN			"warehouse.db"

#define LED_ON 				1
#define LED_OFF 			0
//#define LED_ON 			_IO('L', 10)
//#define LED_OFF 			_IO('L', 11)
//#define BEEP_OFF			_IO('B', 11)	
//#define BEEP_ON			_IO('B', 10)
#define BEEP_OFF			0
#define BEEP_ON				1

//#define MSGTYPE_ENV			'e'
//#define MSGTYPE_GOODS 		'g'
//#define MSGTYPE_ACK			'a'

#define STORAGE_NUM 		5

#define GOODS_NUM			10
#define GOODS_IN 			'I'
#define GOODS_OUT			'O'
/*
#define SMS_TEM				't'
#define SMS_HUM				'h'
#define SMS_ILL				'i'
#define SMS_BRE				'b'
*/
#define SMS_TEM			1
#define SMS_HUM			2	
#define SMS_ILL			3	
#define SMS_BRE			4
#define SMS_STRNO1		0
#define SMS_STRNO2		1<<7

#define SMS_MSG_TEM1	0
#define SMS_MSG_BRE1	1
#define SMS_MSG_ILL1	2
#define SMS_MSG_HUM1	3
#define SMS_MSG_TEM2	4
#define SMS_MSG_BRE2	5
#define SMS_MSG_ILL2	6
#define SMS_MSG_HUM2	7

#define QUEUE_MSG_LEN		32

#define MSG_LED				1L  
#define MSG_LED_TEM_ON		0x22
#define MSG_LED_TEM_OFF		0x20
//#define MSG_LED_TEM_ON		0x01	
//#define MSG_LED_TEM_OFF		2
#define MSG_LED_HUM_ON		0x22
#define MSG_LED_HUM_OFF		0x20
#define MSG_LED_ILL_ON		0x11
#define MSG_LED_ILL_OFF		0x10
#define MSG_LED_TRI_ON		0x11
#define MSG_LED_TRI_OFF		0x10


#define MSG_BEEP			2L
#define MSG_BEEP_OFF		0
#define MSG_BEEP_ON			1

#define MSG_CAMERA			3L


#define MSG_M0				4L
#define MSG_CONNECT_SUCCESS 'Y'
#define MSG_M0_FAN_OFF		0x00
#define MSG_M0_FAN_ON1		0x01
#define MSG_M0_FAN_ON2		0x02
#define MSG_M0_FAN_ON3		0x03

#define MSG_M0_BEEP_OFF		0x10
#define MSG_M0_BEEP_ON		0x11
#define MSG_M0_BEEP_AU_OFF	0x12
#define MSG_M0_BEEP_AU_ON	0x13

#define MSG_M0_LED_OFF		0x20
#define MSG_M0_LED_ON		0x21

#define MSG_M0_SEG_ON		0x30
#define MSG_M0_SEG_OFF		0x3f

#define ENV_UPDATE			0x00
#define ENV_GET				0x01
#define COLLECT_INSERTER	0x10
#define COLLECT_TIME_GET	0x11
#define COLLECT_CURRENT_GET 0x12
#define GOODS_ADD			0x20
#define GOODS_REDUCE		0x21
#define GOODS_GET			0x22

extern void *pthread_sqlite (void *);			//数据库线程
extern void *pthread_analysis (void *);			//数据解析线程
extern void *pthread_transfer (void *);			//数据接收线程
extern void *pthread_uart_cmd (void *);			//命令发送线程
extern void *pthread_client_request (void *);	//接收CGI，QT请求
extern void *pthread_infrared (void *);			//红外线程，用按键模拟
//extern void *pthread_buzzer (void *);			//蜂鸣器控制线程
//extern void *pthread_led (void *);				//LED控制线程
extern void *pthread_camera (void *);			//摄像头线程
extern void *pthread_sms (void *);				//发送短信线程
extern void *pthread_refresh (void *);	//共享内存数据刷新线程

extern void sendMsgQueue (long, unsigned char);

struct storage_goods_info
{
	unsigned char goods_type;
	unsigned int goods_count;
};

struct storage_info
{
	unsigned char storage_status;				// 0:open 1:close
	unsigned char led_status;
	unsigned char buzzer_status;
	unsigned char fan_status;
	unsigned char seg_status;
	signed char x;
	signed char y;
	signed char z;
	char samplingTime[20];
	float temperature;
	float temperatureMIN;
	float temperatureMAX;
	float humidity;
	float humidityMIN;
	float humidityMAX;
	float illumination;
	float illuminationMIN;
	float illuminationMAX;
	float battery;
	float adc;
	float adcMIN;
	struct storage_goods_info goods_info[GOODS_NUM];
};

struct env_info_clien_addr
{
	struct storage_info storage_no[STORAGE_NUM];	
};

struct sqlite_operation
{
	int table_select_mask;
	int env_operation_mask;
	int table_operation_mask;
	int goods_operation_mask;
};

struct msg
{
	long type;
	long msgtype;
	unsigned char text[QUEUE_MSG_LEN];
};

#endif
