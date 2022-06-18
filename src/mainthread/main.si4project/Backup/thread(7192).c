/*
 * (C) Copyright 2022
 * https://gitee.com/yikoulinux
 * 
 * This program is distributed in the purpose for training only
 * and hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Revision history
 * ------------------------------------------------------
 * 20120501	Daniel.Peng	initial 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>   

void *thread_function(void *arg)
{
	static char message[32] = "Hello World";
	
	printf("thread a: address of message = %p\n", message);
	strcpy(message, "marked by thread");
	
	sleep(10);

	pthread_exit(message);
}

int main(int argc,  char *argv[])
{
	pthread_t	a_thread;
	void		*thread_result;

	if (pthread_create(&a_thread, NULL, thread_function, NULL) < 0)
	{  
		perror("fail to pthread_create");
		exit(-1);
	}
	
	printf("thread main: waiting for thread to finish\n");
	if (pthread_join(a_thread, &thread_result) < 0)
	{
		perror("fail to pthread_join");
		exit(-1);
	}
	printf("thread main: value of thread_result = %p, MESSAGE is now %s\n", 
		thread_result, (char *)thread_result);
	
	return 0;
}
          
