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
#include <semaphore.h>

pthread_mutex_t mutex;//锁定标志 
pthread_cond_t cond;//唤醒条件变量


void *thread1_func(void *arg)
{
	int	n;	
	printf("enter t1\n");
	pthread_mutex_lock(&mutex);//锁定	
	printf("t1 lock mutex\n");
	for (n = 0; n < 3; n++) {
		sleep(1);
		printf("AAAAAAAAAA\n");
	}

	pthread_cond_signal(&cond);//唤醒单个线程
	printf("t1 signal cond to t2\n");	
	pthread_mutex_unlock(&mutex);//解锁
	printf("t1 unlock mutex\n");	

	pthread_exit(NULL);
	//return NULL;
}

void *thread2_func(void *arg)
{
	int n;
	
	printf("\t\tenter t2\n");

	pthread_mutex_lock(&mutex);//锁定, 阻塞前被锁上
	printf("\t\tt2 lock mutex and wait\n");	
	pthread_cond_wait(&cond,&mutex);
	printf("\t\tt2 wakeup\n");
	for (n = 0; n < 3; n++) {
		sleep(1);
		printf("\t\tBBBBBBBBBB\n");
	}
	pthread_mutex_unlock(&mutex);//解锁
	printf("\t\tt2 unlock mutex\n");	

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;

	//生成互斥锁mutex,默认属性初始化
	pthread_mutex_init(&mutex,NULL);
	
	//生成一个唤醒变量,默认属性=同一进程内的所有线程使用
	pthread_cond_init(&cond,NULL);

	
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
	
	pthread_cond_destroy(&cond);//释放阻塞唤醒变量
	pthread_mutex_destroy(&mutex);//释放互斥锁mutex资源
	printf("main is exiting.\n");
	
	return 0;
}
