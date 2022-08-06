#include <sys/types.h>
#include <linux/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem.h"

int main(void)
{
	key_t key;   //sem key
	int semid, semval;

	//get semaphore
	if((key = ftok("/app",'i')) <0)
	{
		perror("ftok");
		exit(1);
	}
	printf("key = %x\n",key);

	if((semid  = semget(key, 1, 0666)) < 0)
	{
		perror("semget");
		exit(1);
	}

	//get semaphore value every 1 seconds  
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
			//break;
		}
		sleep(1);
	}
	exit(0);
}
