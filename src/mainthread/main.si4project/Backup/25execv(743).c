#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
int main(int argc,char **argv)
{
	pid_t pid;
	printf("PID = %d\n",getpid());
	pid=fork();
	if(pid==0)
	{//子进程
		execvp("ls",argv);    // ./r -ef		
//		execv("/bin/ls",argv);  ./run -l 
//		execl("/bin/ls","ls","-l","/",NULL);
		//指令路径 指令 参数   目录   空
//		execlp("ls","ls","-al","/",NULL);
		sleep(10);
	}else if(pid!=-1)
	{//父进程		
		printf("\nParrent porcess,PID = %d\n",getpid());
	}else
	{
		printf("error fork() child proess!");
	}
	return 0 ;
}
