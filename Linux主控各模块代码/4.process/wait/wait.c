#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

/* wait */
        
int main(int argc, char **argv)
{
	pid_t pid;
	int status;

	printf("parent[pid=%d] is born\n", getpid());
		 
	if (-1 == (pid = fork())) {
		perror("fork error");
		return -1;
	}
	
	if (pid == 0){
		printf("child[pid=%d] is born\n", getpid());
		sleep(10);
		printf("child is over\n");
		exit(7);
		return 123;
	}
	else{
		pid_t pid_w;
		
		pid_w = wait(&status);//等待子进程退出 0x7b00
		if (pid_w < 0) {
			perror("wait error");
			return 1;
		}
		printf("status=%x \n",status);
		if (WIFEXITED(status)) {
			//正常退出
			status = WEXITSTATUS(status);//提取返回信息
			printf("wait returns with pid = %d. return status is %d\n", pid_w, status);
		} else {
			//非正常退出
			printf("wait returns with pid = %d. the child is terminated abnormally\n", pid_w);
		}		
		printf("father is over\n");		
		return 0;
	}
}
