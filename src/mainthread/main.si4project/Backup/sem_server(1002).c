#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/sem.h>
#include <stdio.h>

#define MAX_RESOURCE    10

int main(void)
{
	key_t key;
	int semid;

	//get semaphore 
	if ((key = ftok(".", 2)) == -1) 
	{
		perror("ftok error!\n");
		exit(1);
	}
	printf("key is %d\n", key);
	if ((semid = semget(key, 1, IPC_CREAT | 0666)) == -1) 
	{
		perror("semget error!\n");
		exit(1);
	}

	//set semphore initial value
	union semun semopts;
	semopts.val = MAX_RESOURCE;
	if (semctl(semid, 0, SETVAL, semopts) == -1) 
	{
		perror("semctl error!\n");
		exit(1);
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
