/**********************************************************************************************
 $$$$$			 $$$$			$$$$$			$$$$$$$$			 $$$$$		  $$$$$$		
 $$$$$$ 		$$$$$$		   $$$$$$		$$$$$$$$$$$$$$$ 		$$$$$$		  $$$$$$		
  $$$$$$	   $$$$$$$		  $$$$$$	  $$$$$$$$$$$$$$$$$ 	   $$$$$$$$ 	 $$$$$$$$		
   $$$$$	  $$$$$$$$$ 	  $$$$$ 	 $$$$$$ 				   $$$$$$$$ 	 $$$$$$$$		
	$$$$$	 $$$$$$$$$$ 	 $$$$$		 $$$$$$$$$$$$$			  $$$$$$$$$$	$$$$$$$$$		
	$$$$$$	 $$$$$ $$$$ 	$$$$$		 $$$$$$$$$$$$$$$$$		  $$$$$$$$$$   $$$$$ $$$$$		
	 $$$$$	$$$$$  $$$$$   $$$$$		   $$$$$$$$$$$$$$$$$	 $$$$$$$$$$$$ $$$$$$ $$$$$		
	  $$$$ $$$$$$  $$$$$  $$$$$ 					 $$$$$$$$	 $$$$$	$$$$$ $$$$$  $$$$$		
	  $$$$$$$$$$	$$$$$$$$$$		   $$$				$$$$$	$$$$$	$$$$$$$$$$	  $$$$$ 	 
	   $$$$$$$		$$$$$$$$$		   $$$$$$		$$$$$$$$   $$$$$$	 $$$$$$$$	  $$$$$$   
	   $$$$$$$		 $$$$$$$		   $$$$$$$$$$$$$$$$$$$$    $$$$$	  $$$$$$$	   $$$$$$   
		$$$$$		 $$$$$$ 			 $$$$$$$$$$$$$$$$	   $$$$$	  $$$$$$	   $$$$$$	
                                                                                          
 * Copyright (C) 2022 - 2023, HaoQing, <970407688@qq.com>.
 * 		<https://github.com/wuxiaolie/WirelessSersorMS>   
 * 		<https://gitee.com/yang-haoqing/wireless-sersor-ms>
 *
 * This software is licensed as described in the file COPYING, which you should have received 
 * as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell copies of 
 * the Software, and permit persons to whom the Software is furnished to do so, under 
 * the terms of the COPYING file.
 *
 *********************************************************************************************/

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


#define DEV_ZIGBEE 			"/dev/ttyUSB0"
#define DEV_CAMERA			"/dev/video0"


#define BEEP_OFF			0
#define BEEP_ON				1

//#define MSGTYPE_ENV			'e'
//#define MSGTYPE_GOODS 		'g'
//#define MSGTYPE_ACK			'a'

#define ENDPOIT_NUM 		4




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



#define MSG_BEEP_OFF		0
#define MSG_BEEP_ON			1


//主线程、消息队列
#define MSG_LED				1L  
#define MSG_BEEP			2L
#define MSG_CAMERA			3L
#define MSG_ZIGBEE			4L


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



#define QUEUE_MSG_LEN 32


#define LEN_ENV 20  //struct getEnvMsg 

/* 长度与zigbee中下面结构体保持一致
cc2530  int == 16位

struct _EnvMsg
{
	uint8_t head[3];	//标识位st:   73 74 3A 65 
	uint8_t type;			//数据类型

	uint8_t temp[2];	//温度 
	uint8_t hum[2];		//湿度 
	uint8_t ep_no;			//仓库编号    01
	
	int8_t x;			 		//三轴信息
	int8_t y;			 
	int8_t z;
	unsigned int hongwai;    
	unsigned int lux;			//光照  2个字节 
	unsigned int rsv2;
	unsigned int gas;			//烟雾 
	unsigned int rsv3;
	unsigned int adc; 		//电位器信息
};

*/

extern void *pthread_zigbee_rcv (void *);			//数据接收线程
extern void *pthread_uart_send (void *);			//命令发送线程
extern void *pthread_main (void *);	//接收CGI，QT,app线程请求
extern void *pthread_camera (void *);			//摄像头线程
extern void *pthread_refresh (void *);	//共享内存数据刷新线程

extern void sendMsgQueue (long, unsigned char);

struct env_info
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
	unsigned int hongwai;
	unsigned int illumination;
	float illuminationMIN;
	float illuminationMAX;
	unsigned int gas;
	float adc;
	float adcMIN;
};

struct env_info_array
{
	struct env_info env_no[ENDPOIT_NUM];	
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
