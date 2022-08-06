/*
 * (C) Copyright 2014
 * http://www.farsight.com.cn/
 * http://nj.hqyj.com/
 * 
 * This program is distributed in the purpose for training only
 * and hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Revision history
 * ------------------------------------------------------
 * 20140928	unicornx	initial archived
 * 20150521	unicornx	added while(1) for ps
 *				while the threads are running
 */
/*
 * This sample demos how to create threads w/ pthread_create().
 * For thread 1, demos how to pass argument into thread working 
 * routine and how to modify global var in thread working routine.
 * For thread 2, demos how to pass structral argument into
 * thread routine.
 * Run this program several times, you may see thread1 or thread2
 * both has chance to print out in the first position.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>  

struct thread_arg {
	int	arg1;
	int	arg2;	
};

char message[30] = "AAAAAAAAAA";

void *t1_routine(void *arg)
{
	printf("thread 1 is running, argument is:%s\n", 
		(char *)arg);
	strcpy(message,  "BBBBBBBBBB");
	
	printf("thread 1 is exiting...\n");
	
	while(1);
	
	return NULL;
}

void *t2_routine(void *arg)
{
	struct thread_arg *parg = (struct thread_arg *)arg;
	
	printf("thread 2 is running, arg1 = %d, arg2 = %d\n", 
		parg->arg1, 
		parg->arg2);
	
	printf("thread 2 is exiting...\n");
	
	while(1);
	
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t	thread_1, thread_2;
	int		retval;
	
	printf("main thread is running ...\n");
	
	retval = pthread_create(&thread_1, 
				NULL, 
				t1_routine, 
				(void *)message); 
	if (retval != 0) {  
		perror("pthread_create error");
		exit(-1);
	}
	
	struct thread_arg arg;
	arg.arg1 = 100;
	arg.arg2 = 200;
	retval = pthread_create(&thread_2, 
				NULL, 
				t2_routine, 
				(void *)&arg); 
	if (  retval != 0) {  
		perror("pthread_create error");
		exit(1);
	}
	
	/* 
	 * let main thread fly a while to allow child threads
	 * get chance to print out sth on tty.
	 */
	sleep(2);

	printf("global string is updated to:%s\n", 
		message);
		
	printf("main thread is exiting...\n");
	
	while(1);
	
	return 0;
}
