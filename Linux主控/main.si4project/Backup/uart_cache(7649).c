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


