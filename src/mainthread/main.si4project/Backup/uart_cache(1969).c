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

/*
从消息队列获取的命令存放的链表相关操作
*/

#include "uart_cache.h"

uart_cache_list CreateEmptyCacheList ()
{
	uart_cache_list h;
	h = (uart_cache_list) malloc (sizeof (uart_cache_node));
	h->next = NULL;
	return h;
}


int EmptyCacheList (uart_cache_list h)
{
	return NULL == h->next;
}

uart_cache_list GetCacheNode (uart_cache_list h, uart_cache_list *t)
{
	if (1 == EmptyCacheList (h))
	{
		return NULL;
	}

	uart_cache_list p = h->next;
	h->next = p->next;
	if (p->next == NULL)
	{
		*t = h;
	}
	return p;
}

int InsertCacheNode (uart_cache_list *t, uart_cache_data x)
{
	uart_cache_list q = (uart_cache_list)malloc (sizeof (uart_cache_node));
	if (NULL == q)
	{
		printf ("InsertCacheNode Error\n");
		return -1;
	}
	(*t)->next = q;
	*t = q;
	q->data = x;
	q->next = NULL;

	return 0;
}


