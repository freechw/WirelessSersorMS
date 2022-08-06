#include <sys/types.h> 
#include <unistd.h> 


int main() 
{ 
	if(fork()) { 
		// 父进程


	}else{ 
		// 子进程

		while(1){ 
			sleep(1); 
		}  
	} 
} 
