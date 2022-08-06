#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

/* wait method: */
 /* waitpid*/
int main(int argc, char **argv)
{
	pid_t pid;

	printf("parent[pid=%d] is born\n", getpid());
		 
	if (-1 == (pid = fork())) {
		perror("fork error");
		return -1;
	}
	
	if (pid == 0){//子进程
		printf("child[pid=%d] is born\n", getpid());
		sleep(5);
		printf("child is over\n");
	}
	else{ //parent  父进程
		pid_t pid_w;
		
		while((pid_w = waitpid(pid, NULL, WNOHANG)) == 0) {
			printf("parent wait w/o HAND and returns with 0\n");
			sleep(1);
		}
		printf("waitpid returns with pid = %d.\n", pid_w);
		
		printf("father is over\n");
	}
	
	return 0;
}

