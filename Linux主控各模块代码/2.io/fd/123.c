#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<pthread.h>
#include <signal.h>
#include <errno.h>


#define MAX 10

int main(int argc, char **argv)
{
	int fd1,fd2,fd3;

	fd1 = MAX;

	fd1 =  open("test1",O_RDWR);
	if(fd1<0)
	{
		perror("open fail\n");
		//printf("open fail %d errno=%d\n",fd1,errno);
	}

	return 0;
}
