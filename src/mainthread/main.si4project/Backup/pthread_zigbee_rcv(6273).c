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
#include "link_list.h"
#include "data_global.h"

#define LEN_ENV 20  //struct getEnvMsg 

/* 长度与zigbee中下面结构体保持一致
struct _EnvMsg
{
	uint8_t head[3];	//标识位st:
	uint8_t type;			//数据类型
	
	uint8_t ep_no;			//仓库编号
	uint8_t temp[2];	//温度
	uint8_t hum[2];		//湿度
	int8_t x;			 		//三轴信息
	int8_t y;			 
	int8_t z;			 
	uint32_t lux;			//光照
	uint32_t bet;			//烟雾
	uint32_t adc; 		//电位器信息
};

*/

extern int dev_uart_fd;

extern linklist envlinkHead;
extern pthread_mutex_t mutex_linklist;
extern pthread_cond_t cond_refresh;

void serial_init(int fd)
{
	struct termios options;
	tcgetattr(fd, &options);
	options.c_cflag |= ( CLOCAL | CREAD );
	options.c_cflag &= ~CSIZE;
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CSTOPB; 
	options.c_iflag |= IGNPAR;
	options.c_iflag &= ~(ICRNL | IXON);
	options.c_oflag = 0;
	options.c_lflag = 0;

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	tcsetattr(fd,TCSANOW,&options);
}

void *pthread_zigbee_rcv (void *arg)
{
	int i = 0, len;
	char flag = 0, check;
	link_datatype buf;

	envlinkHead = CreateEmptyLinklist ();
#if 1
	if ((dev_uart_fd = open (DEV_ZIGBEE, O_RDWR)) < 0)
	{
		perror ("open ttyUSB0 fail");
	//	exit (-1);
	//	return -1;
	}
	serial_init (dev_uart_fd);

	printf ("pthread_transfer is ok\n");

#endif
	while (1)
	{
		memset (&buf, 0, sizeof (link_datatype));
		read (dev_uart_fd, &check, 1);
        if (check == 'c')
        {
            sendMsgQueue(MSG_ZIGBEE,MSG_CONNECT_SUCCESS);
        }
		if (check == 's')
		{
			check = 0;
			read (dev_uart_fd, &check, 1);
			if (check == 't')
			{
				check = 0;
				read (dev_uart_fd, &check, 1);
				if (check == ':')
				{
					check = 0;
					read (dev_uart_fd, &check, 1);
					if (check == 'e')
					{
						buf.msg_type = 'e';
						usleep(1);
						if ((len = read (dev_uart_fd, buf.text, LEN_ENV)) != LEN_ENV)
						{
							for (i = len; i < LEN_ENV; i++)
							{
								read (dev_uart_fd, buf.text+i, 1);
							}
						}
						flag = 1;
					}			
				}
			}
		}
		
		if (1 == flag)
		{
			pthread_mutex_lock (&mutex_linklist);
			//接收到的额数据加入到链表中
			if ((InsertLinknode (buf)) == -1)
			{
				pthread_mutex_unlock (&mutex_linklist);
				printf ("NONMEM\n");
			}
			pthread_mutex_unlock (&mutex_linklist);
			flag = 0;
			pthread_cond_signal (&cond_refresh);
		}
		
	}
	return 0;
}
