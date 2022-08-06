#include <stdio.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "data_global.h"
#include "sem.h"

#define N 1024

struct shm_addr
{
	char cgi_status;
	char qt_status;
	struct env_info_array rt_status;
};


int main() 
{
	key_t key_info;
	int shmid, semid;


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

	struct env_info *current =  &shm_buf->rt_status.env_no[1];

	while (1)
	{
		//共享内存shm_buf需要由进程信号量来控制互斥访问
		sem_p (semid, 0);

		printf ("tem = %0.2f hum = %0.2f ill = %d  gas = %d hongwai=%u\n", 
				current->temperature, 
				current->humidity, 
				current->illumination, 
				current->gas,
				current->hongwai);
		
		sem_v (semid, 0);
		sleep(3);
	}
	return 0;
} 
