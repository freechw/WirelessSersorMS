#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//	 int open(const char *pathname, int flags);
//	 int open(const char *pathname, int flags, mode_t mode);


#define PATH "yikou"

#define MAX_BUF_LEN 50

int main(void)
{
	int fd;
	int len =0;
	char buf[MAX_BUF_LEN]="hello linus\n";
	char tbuf[MAX_BUF_LEN]={0};

	fd = open(PATH,O_RDWR | O_CREAT,0666);
	if(fd < 0)
	{
		perror("open fail\n");
		return -1;
	}

	printf("open ok\n");


	len = write(fd,buf,strlen(buf));
	printf("len=%d\n",len);


	lseek(fd,4096*2,SEEK_SET);
	
	len = write(fd,buf,strlen(buf));
	printf("len=%d\n",len);

	close(fd);
}
