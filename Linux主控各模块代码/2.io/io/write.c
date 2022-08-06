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
	int fd,pos,nbyres;
	ssize_t bytes_written;
	char buf[512]={0};
	
	fd = open(filename,O_RDWR | O_CREAT | O_TRUNC,mode);
	if(fd == -1)
	{
		printf("open file err \n");
		return -1;
	}
 
	strcpy(buf,"this is a test\n");
	nbyres = strlen(buf);
	
	bytes_written = write(fd,buf,nbyres);
	
	printf("nbyres: %d  bytes_written:%d \n",nbyres,bytes_written);
 
	if(close(fd) < 0)
	{
		fprintf(stderr,"ERROR: close file %s failed:%s\n",filename,strerror(errno));
		return -1;
	}
//	printf("%s\n",buf);
	return 0;
}