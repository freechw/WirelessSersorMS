/* shm_client.c : the time client using shared memory, a bizarre application  */
#include        <stdlib.h>
#include        <unistd.h>
#include	<stdio.h>
#include	<sys/shm.h>
#include	<time.h>

#define	TIME_MEM_KEY	99
#define	SEG_SIZE	((size_t)100)


main()
{
	key_t key_info;
	
	if ((key_info = ftok ("/app", 'i')) < 0)
	{
		perror ("ftok info");
		exit (-1);
	}
	int seg_id = shmget(key_info, SEG_SIZE, 0777);
	
	char *mem_ptr = shmat(seg_id, NULL, 0);
	
	printf("The time, direct from memory: ..%s", mem_ptr);
	shmdt(mem_ptr);
}
