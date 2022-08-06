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

#ifndef __UART_CACHE_H__
#define __UART_CACHE_H__

#include <stdio.h>
#include <stdlib.h>
#include "uart_cache.h"

typedef void* uart_cache_data;

typedef struct _uart_node_
{
	uart_cache_data data;
	struct _uart_node_ *next;
}uart_cache_node, *uart_cache_list;

extern uart_cache_list CreateEmptyCacheList ();
extern int EmptyCacheList (uart_cache_list);
extern uart_cache_list GetCacheNode (uart_cache_list, uart_cache_list *);
extern int InsertCacheNode (uart_cache_list *, uart_cache_data);

#endif
