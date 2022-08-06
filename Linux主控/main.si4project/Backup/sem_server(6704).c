#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/sem.h>
#include <stdio.h>
#include "sem.h"

#define MAX_RESOURCE    10

int main(void)
{
	key_t key;
	int semid;

	//get semaphore 
	if ((key = ftok ("/app", 'i')) < 0)
	{
		perror ("ftok info");
		exit (-1);
	}

	
	printf("key is %d\n", key);

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

	//substract 1 every 3 seconds until semaphore value is -1
	struct sembuf sbuf = { 0, -1, IPC_NOWAIT };
	while (1) 
	{
		//p
		if (semop(semid, &sbuf, 1) == -1) 
		{
			perror("semop error!\n");
			exit(1);
		}
		
		//进入临界区
		
		//V
		
		sleep(3);
	}
	exit(0);
}
