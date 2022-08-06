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


