#include <sys/types.h> 
#include <unistd.h> 

/* create a ZOMBIE 
 * ps -ax | grep a.out to show the zombie 
 */ 
int main() 
{ 
	if(fork()) { 
		// 父进程

		while(1){ 
			sleep(1); 
		} 
	} 
	// 子进程
} 
