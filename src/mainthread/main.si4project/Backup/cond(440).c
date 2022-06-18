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


void *thread1_func(void *arg)
{
	int	n;	
	
	for (n = 0; n < 30; n++) {
		sleep(1);
		printf("AAAAAAAAAA\n");
	}

	pthread_exit(NULL);
	//return NULL;
}

void *thread2_func(void *arg)
{
	int n;

	for (n = 0; n < 30; n++) {
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











char buffer[128];
int buffer_has_data=0;
pthread_mutex_t mutex;//锁定标志 
pthread_cond_t cond;//唤醒条件变量

void write_buffer(char *data)
{
	pthread_mutex_lock(&mutex);//锁定
	if(buffer_has_data==0)
	{
		sprintf(buffer,"%s",data);
		buffer_has_data=1;
		pthread_cond_signal(&cond);//唤醒单个线程
		//pthread_cond_broadcast(&cond);//唤醒所有线程,test1,test2
	}
	printf("write mutex unlock\n");
	pthread_mutex_unlock(&mutex);//解锁
}

void read_buffer(void) //
{
	while(1)
	{
		pthread_mutex_lock(&mutex);//锁定, 阻塞前被锁上
		//无数据时,一般用循环体阻塞,
		//这样可以判断当前条件是否满足唤醒,否则继续阻塞,
		//这样在多线程相互作用时,比较稳定
		while(!buffer_has_data)
		{	//阻塞线程,等待唤醒 和 解锁mutex, 注意有2个参数，
			//第一个是唤醒变量cond,第2个是解锁mutex
			printf("read cond wait \n");
			pthread_cond_wait(&cond,&mutex);
		}
		printf("read buffer,data = %s\n",buffer);
		buffer_has_data=0;
		pthread_mutex_unlock(&mutex);//解锁
	}
}
test1()
{
	while(1)
	{//阻塞线程,等待唤醒 和 解锁mutex, 注意有2个参数，
	//第一个是唤醒变量cond,第2个是斥锁mutex
		pthread_cond_wait(&cond,&mutex);
		printf("test1 run\n");
		buffer_has_data=0;
		pthread_mutex_unlock(&mutex);//解锁
	}	
}

test2()
{
	while(1)
	{//阻塞线程,等待唤醒 和 解锁mutex, 注意有2个参数，
	//第一个是唤醒变量cond,第2个是斥锁mutex
		pthread_cond_wait(&cond,&mutex);
		printf("test2 run\n");
		buffer_has_data=0;
		pthread_mutex_unlock(&mutex);//解锁
	}
}

int main(int argc,char **argv)
{
	char input[128];
	pthread_t reader,t1,t2;
	
	//生成互斥锁mutex,默认属性初始化
	pthread_mutex_init(&mutex,NULL);
	
	//生成一个唤醒变量,默认属性=同一进程内的所有线程使用
	pthread_cond_init(&cond,NULL);
	
	//(标识符,线程属性<一般不设为NULL>,运行的函数地址,函数的参数)
	pthread_create(&reader,NULL,(void*)(read_buffer),NULL);
	//测试一次唤醒多个线程
	pthread_create(&t1,NULL,(void*)(test1),NULL);
	//测试一次唤醒多个线程
	pthread_create(&t2,NULL,(void*)(test2),NULL);

	while(1)
	{
		scanf("%s",input);
		write_buffer(input);
	}
	
	pthread_join(reader,NULL);//主线程等待reader线程退出
	pthread_cond_destroy(&cond);//释放阻塞唤醒变量
	pthread_mutex_destroy(&mutex);//释放互斥锁mutex资源
		

	return 0;
}

#if 0

无论哪种等待方式，都必须和一个互斥锁配合，
以防止多个线程同时请求pthread_cond_wait()（或pthread_cond_timedwait()，下同）的竞争条件（RaceCondition）。
mutex互斥锁必须是普通锁（PTHREAD_MUTEX_TIMED_NP）或者适应锁（PTHREAD_MUTEX_ADAPTIVE_NP），且在调用pthread_cond_wait()前必须由本线程加锁（pthread_mutex_lock()），而在更新条件等待队列以前，mutex保持锁定状态，并在线程挂起进入等待前解锁。
在条件满足从而离开pthread_cond_wait()之前，mutex将被重新加锁，以与进入pthread_cond_wait()前的加锁动作对应。
#endif
