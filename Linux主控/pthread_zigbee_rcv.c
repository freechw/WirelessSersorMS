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

#include "link_list.h"
#include "data_global.h"



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
