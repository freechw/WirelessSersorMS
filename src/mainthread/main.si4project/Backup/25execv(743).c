#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
int main(int argc,char **argv)
{
	pid_t pid;
	printf("PID = %d\n",getpid());
	pid=fork();
	if(pid==0)
	{//�ӽ���
		execvp("ls",argv);    // ./r -ef		
//		execv("/bin/ls",argv);  ./run -l 
//		execl("/bin/ls","ls","-l","/",NULL);
		//ָ��·�� ָ�� ����   Ŀ¼   ��
//		execlp("ls","ls","-al","/",NULL);
		sleep(10);
	}else if(pid!=-1)
	{//������		
		printf("\nParrent porcess,PID = %d\n",getpid());
	}else
	{
		printf("error fork() child proess!");
	}
	return 0 ;
}
