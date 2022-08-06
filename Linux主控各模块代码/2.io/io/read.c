#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include<errno.h>
extern int errno;

mode_t mode = 0444;

char *filename = "peng";

 

int main()
{
	int fd,pos;
	ssize_t bytes_read;
	char buf[512]={0};
	
	fd = open(filename,O_RDONLY,mode);
	if(fd == -1)
	{
		printf("open file err \n");
		return -1;
	}
	
	bytes_read = read(fd,buf,sizeof(buf));
	
	printf("buf: %s  \nbytes_written:%d \n",buf,bytes_read);
	
	if(close(fd) < 0)
	{
		fprintf(stderr,"ERROR: close file %s failed:%s\n",filename,strerror(errno));
		return -1;
	}


	return 0;
}