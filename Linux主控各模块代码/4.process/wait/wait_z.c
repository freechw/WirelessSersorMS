#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

/* no wait and let zombie be */

int main(int argc, char **argv)
{
	pid_t pid;
	
	printf("parent[pid=%d] is born\n", getpid());
		 
	if (-1 == (pid = fork())) {
		perror("fork error");
		return -1;
	}
	
	if (pid == 0){
		printf("child[pid=%d] is born\n", getpid());
		sleep(10);
		printf("child is over\n");
	}
	else{
		while(1){};
	}
		
	return 0;
}
