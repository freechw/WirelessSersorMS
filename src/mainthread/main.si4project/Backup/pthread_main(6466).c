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

extern unsigned char dev_led_mask;
extern unsigned char dev_camera_mask;
extern unsigned char dev_buzzer_mask;
extern unsigned char dev_uart_mask;

extern pthread_cond_t cond_camera;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_uart_cmd;
extern pthread_cond_t cond_sqlite;

extern pthread_mutex_t mutex_global;
extern pthread_mutex_t mutex_uart_cmd;
extern pthread_mutex_t mutex_camera;
extern pthread_mutex_t mutex_slinklist;

extern char cgi_status;
extern int msgid;
extern struct env_info_array all_info_RT; 

extern uart_cache_list zigbee_cache_head, zigbee_cache_tail;
struct setEnv
{
	int temMAX;
	int temMIN;
	int humMAX;
	int humMIN;
	int illMAX;
	int illMIN;
};



void *pthread_main (void *arg)
{
	key_t key;
	ssize_t msgsize;
	struct msg msgbuf;

	if ((key = ftok ("/app", 'g')) < 0)
	{
		perror ("ftok msgqueue");
		exit (-1);
	}
	if ((msgid = msgget (key, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		if(errno == EEXIST)
		{
			msgid = msgget (key,0666);
			return 0;
		} 
		else
		{
			perror ("msgget msgid");
			exit (-1);
		}
	}

	zigbee_cache_head = CreateEmptyCacheList ();
	zigbee_cache_tail = zigbee_cache_head;
	unsigned char *zigbee_temp;

	printf ("pthread_main is ok\n");
	while (1)
	{
		bzero (&msgbuf, sizeof (msgbuf));
		printf ("\nwait for the msg\n");
		msgsize = msgrcv (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 1L, 0);
		
		printf ("  Get %ldL msg [%ld]\n", msgbuf.msgtype,msgsize);
		printf ("  text[0] = %#x\n", msgbuf.text[0]);

		switch (msgbuf.msgtype)
		{
			case MSG_CAMERA:
			{//发送给camera控制线程
				pthread_mutex_lock (&mutex_camera);
				dev_camera_mask = msgbuf.text[0];
				pthread_cond_signal (&cond_camera);
				pthread_mutex_unlock (&mutex_camera);
				break;
			}
			case MSG_ZIGBEE:
			{//zigbee发送命令
				//usleep (200000);
				pthread_mutex_lock (&mutex_uart_cmd);	
				zigbee_temp = (unsigned char *)malloc (sizeof (unsigned char));
				*zigbee_temp = msgbuf.text[0];
				printf("  msgbuf.text = %x\n",msgbuf.text[0]);
				InsertCacheNode (&zigbee_cache_tail, zigbee_temp);
				//dev_uart_mask = msgbuf.text[0];
				pthread_mutex_unlock (&mutex_uart_cmd);
				pthread_cond_signal (&cond_uart_cmd);
				break;
			}
			default : 
				break;
		}
	}

}
