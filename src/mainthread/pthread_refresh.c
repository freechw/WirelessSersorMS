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
 #include <arpa/inet.h>

#include "sem.h"
#include "data_global.h"
#include "link_list.h"

#define N 1024

extern pthread_mutex_t mutex_refresh;
extern pthread_mutex_t mutex_refresh_updata;
extern pthread_mutex_t mutex_global;
extern pthread_mutex_t mutex_slinklist;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_refresh_updata;
extern pthread_mutex_t mutex_linklist;

extern char qt_status;
extern int shmid; 
extern int semid;
extern struct env_info_array all_info_RT;
extern linklist envlinkHead, envlinkTail;

struct shm_addr
{
	char cgi_status;
	char qt_status;
	struct env_info_array rt_status;
};


struct getEnvMsg
{
	unsigned char tem[2];
	unsigned char hum[2];
	unsigned char ep_no;	
	unsigned char x;
	unsigned char y;
	unsigned char z;

	unsigned short hongwai;
	unsigned short lux;			//光照  2个字节
	unsigned short rsv2;
	unsigned short gas;			//烟雾
	unsigned short rsv3;
	unsigned short adc;			

};
void getEnvPackage (link_datatype *buf)
{
	unsigned short tmp;
	struct getEnvMsg pack;
	float temh,teml;
	int ep_no;
	
	memcpy (&pack, buf->text, LEN_ENV);
	ep_no = pack.ep_no;

	pthread_mutex_lock (&mutex_global);
	struct env_info current = all_info_RT.env_no[ep_no];
	pthread_mutex_unlock (&mutex_global);

	current.storage_status = 1;
	current.x = pack.x;
	current.y = pack.y;
	current.z = pack.z;
	
	temh =  pack.tem[1];
	teml =  pack.tem[0];
	current.temperature = temh;

	temh =  pack.hum[1];
	teml =  pack.hum[0];
	current.humidity = temh;
	
	current.illumination = (unsigned short)pack.lux;//光强

	current.gas = (unsigned short)pack.gas;//烟雾

	current.hongwai = pack.hongwai;
	
	printf ("ep_no = %d tem = %0.2f hum = %0.2f ill = %d battery = %d gas = %0.2f hongwai=%d\n", 
			ep_no,current.temperature, current.humidity, current.illumination, current.gas,current.hongwai);

	//checkEnv (sto_no, &current);	

	pthread_mutex_lock (&mutex_global);
	all_info_RT.env_no[ep_no] = current;
	pthread_mutex_unlock (&mutex_global);



	return ;
}

void *pthread_refresh (void *arg)
{
	key_t key_info;
	int shmid, semid;
	linklist node;
	link_datatype buf;

	struct shm_addr *shm_buf;

	if ((key_info = ftok ("/app", 'i')) < 0)
	{
		perror ("ftok info");
		exit (-1);
	}

	if ((semid = semget (key_info, 1, IPC_CREAT | IPC_EXCL |0666)) < 0)
	{
		if (errno == EEXIST)
		{
			semid = semget (key_info, 1, 0666);
		}
		else
		{
			perror ("semget");
			exit (-1);
		}
	}
	else
	{
		init_sem (semid, 0, 1);
	}

	if ((shmid = shmget (key_info, N, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		if (errno == EEXIST)
		{
			shmid = shmget (key_info, N, 0666);
			shm_buf = (struct shm_addr *)shmat (shmid, NULL, 0);
			
		}
		else
		{
			perror ("shmget");
			exit (-1);
		}

	}
	else
	{
		if ((shm_buf = (struct shm_addr *)shmat (shmid, NULL, 0)) == (void *)-1)
		{
			perror ("shmat");
			exit (-1);
		}
	}

	bzero (shm_buf, sizeof (struct shm_addr));
	printf ("pthread_refresh is ok\n");

	while (1)
	{
		pthread_mutex_lock (&mutex_refresh);
		pthread_cond_wait (&cond_refresh, &mutex_refresh);
		while (1)
		{
			pthread_mutex_lock (&mutex_linklist);
			if ((node = GetLinknode (envlinkHead)) == NULL)
			{
				pthread_mutex_unlock (&mutex_linklist);
				break;
			}
			buf = node->data;
			free (node);
			pthread_mutex_unlock (&mutex_linklist);

			if ('e' == buf.msg_type)
			{
				getEnvPackage (&buf);
			}
		}
		//共享内存shm_buf需要由进程信号量来控制互斥访问
		sem_p (semid, 0);
		pthread_mutex_lock (&mutex_global);
		shm_buf->rt_status = all_info_RT;
		pthread_mutex_unlock (&mutex_global);
		sem_v (semid, 0);
		pthread_mutex_unlock (&mutex_refresh);
	}
	return 0;
}

