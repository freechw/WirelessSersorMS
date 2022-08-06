#include <sys/ipc.h>
 #include <sys/shm.h>
#include <stdio.h>
#include <sys/stat.h>
 int shmctl(int shmid, int cmd, struct shmid_ds *buf);


int main()
{
	int  segment_id,segment_size;
	char *shared_memory;
	struct shmid_ds shmbuffer;
	const int shared_segment_size = 0x6400;
/*分配*/
	segment_id = shmget (IPC_PRIVATE, shared_segment_size, 
	                  IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR ); 


	shared_memory = (char*) shmat (segment_id, 0, 0); 
	printf ("shared memory attached at address %p\n", shared_memory); 


	shmctl (segment_id, IPC_STAT, &shmbuffer); 

	segment_size = shmbuffer.shm_segsz; 
	printf ("segment size: %d\n", segment_size); 
	sprintf (shared_memory, "Hello, world."); 

	shmdt (shared_memory); 

/*绑定到共享内存块*/
	shared_memory = (char*) shmat (segment_id, (void*) 0x500000, 0); 
	printf ("shared memory reattached at address %p\n",shared_memory); 

	printf ("%s\n", shared_memory);
	
	/*tuo li*/
	shmdt (shared_memory); 
	/*释放内存*/
	shmctl (segment_id, IPC_RMID, 0); 
	return;

}

