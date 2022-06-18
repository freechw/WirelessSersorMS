/*
 * 无线传感网综合项目主控程序.
 * 	Copyright (C) 2022  Daniel Peng
 *
 * 	This program is free software; you can redistribute it and/or
 * 	modify it under the terms of the GNU General Public License
 * 	as published by the Free Software Foundation; either version 2
 * 	of the License, or (at your option) any later version.
 *
 * 	This program is distributed in the hope that it will be useful,
 * 	but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 	GNU General Public License for more details.
 *
 *
 * Additional updates, Copyright:
 *	Daniel Peng <21689991@qq.com>
 */

#include "data_global.h"
#include "uart_cache.h"

//条件变量
pthread_cond_t cond_zigbee_rcv;
pthread_cond_t cond_uart_cmd;
pthread_cond_t cond_main_thread;
pthread_cond_t cond_camera;
pthread_cond_t cond_refresh;
pthread_cond_t cond_refresh_updata;

//互斥锁
pthread_mutex_t mutex_slinklist;
pthread_mutex_t mutex_zigbee_rcv;
pthread_mutex_t mutex_uart_cmd;
pthread_mutex_t mutex_main_thread;
pthread_mutex_t mutex_camera;
pthread_mutex_t mutex_refresh;
pthread_mutex_t mutex_refresh_updata;
pthread_mutex_t mutex_global;   //对应all_info_RT
pthread_mutex_t mutex_linklist;

unsigned char dev_camera_mask;
unsigned char dev_uart_mask;

uart_cache_list zigbee_cache_head, zigbee_cache_tail;

int dev_camera_fd;
int dev_uart_fd;   //"ttyUSB0"

int msgid; //消息队列
int shmid; //共享内存
int semid; //信号量


/**** sqlite module ****/
int storageNum;

struct env_info_array all_info_RT; //用于对应共享内存中数据

void sendMsgQueue (long type, unsigned char text)
{
	struct msg msgbuf;
	msgbuf.type = 1L;
	msgbuf.msgtype = type;
	msgbuf.text[0] = text;
	msgsnd (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 0);
}
