#include <sys/types.h>
#include <linux/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	key_t key;   //sem key
	int semid, semval;

	//get semaphore
	if ((key = ftok(".", 2)) == -1) 
	{
		perror("ftok error!\n");
		exit(1);
	}
	printf("key is:%d\n", key);
	if ((semid = semget(key, 1, IPC_CREAT | 0666)) == -1) 
	{
		perror("semget error!\n");
		exit(1);
	}

	//get semaphore value every 3 seconds until semaphore value is -1
	while (1) 
	{
		if ((semval = semctl(semid, 0, GETVAL, 0)) == -1) 
		{
			perror("semctl error!\n");
			exit(1);
		}

		if (semval > 0) 
		{
			printf("Still %d resources can be used\n", semval);
		}else 
		{
			printf("No more resources can be used!\n");
			break;
		}
		sleep(3);
	}
	exit(0);
}
