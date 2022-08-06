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

extern unsigned char dev_uart_mask;
extern int dev_uart_fd;

extern pthread_cond_t cond_uart_cmd;

extern pthread_mutex_t mutex_uart_cmd;

extern uart_cache_list zigbee_cache_head, zigbee_cache_tail;

void *pthread_uart_send (void *arg)
{
	unsigned char *uart_p = NULL;
	uart_cache_list uart_cache_p = NULL;
	printf ("pthread_uart_send is ok\n");
	while (1)
	{
		pthread_mutex_lock (&mutex_uart_cmd);
		pthread_cond_wait (&cond_uart_cmd, &mutex_uart_cmd);
		while ((uart_cache_p = GetCacheNode (zigbee_cache_head, &zigbee_cache_tail)) != NULL)
		{
			pthread_mutex_unlock (&mutex_uart_cmd);
			uart_p = (unsigned char *)uart_cache_p->data;
			dev_uart_mask = *uart_p;

			write (dev_uart_fd, &dev_uart_mask, 1);

			printf("\tuart:m0 cmd = %x\n", dev_uart_mask);
			free (uart_p);
			uart_p = NULL;
			free (uart_cache_p);
			uart_cache_p = NULL;
			usleep (200000);
			pthread_mutex_lock (&mutex_uart_cmd);
		}
		pthread_mutex_unlock (&mutex_uart_cmd);
	}
}
