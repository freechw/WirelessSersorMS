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
