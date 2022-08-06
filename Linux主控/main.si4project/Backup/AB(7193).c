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
 * 20150303	unicornx	removed print of thread id
 */
/*
 * This sample demo how to create two threads A & B.
 * Thread A print 3 lines on stdout, each line contains
 * 10 'A' char, between each line sleep 1 sec.  Thread B 
 * does almost the same thing except each line contains 10 
 * 'B' char.
 * Run this program several times, check the output on
 * screen, you may see A line and B line may occur alternately.
 */
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread1_func(void *arg)
{
	int	n;	
	
	for (n = 0; n < 3; n++) {
		sleep(1);
		printf("AAAAAAAAAA\n");
	}

	pthread_exit(NULL);
	//return NULL;
}

void *thread2_func(void *arg)
{
	int n;

	for (n = 0; n < 3; n++) {
		sleep(1);
		printf("BBBBBBBBBB\n");
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	
	if (pthread_create(&tid1, NULL, thread1_func, NULL) != 0) {
		perror("main: pthread_create thread_1 failed");
		return 1;
	} else {
		printf("main: pthread_create thread_1 succeed!\n");
	}

	if (pthread_create(&tid2, NULL, thread2_func, NULL) != 0) {
		perror("main: pthread_create thread_2 failed");
		return 1;
	} else {
		printf("main: pthread_create thread_2 succeed!\n");
	}
	
	if (pthread_join(tid1, NULL) != 0) {
		perror("main: pthread_join thread_1 failed");
	}
	if (pthread_join(tid2, NULL) != 0) {
		perror("main: pthread_join thread_2 failed");
	}
	
	printf("main is exiting.\n");
	
	return 0;
}
 
