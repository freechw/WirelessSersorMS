#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
 
typedef struct stat ST;
 
int main()
{
    //准备结构体
    ST s;
    //调用stat函数
    int res = stat("t.log",&s);
    if(-1 == res)
        perror("error:"),exit(-1);
 

/*
	 
	long		st_atime;	  Time of last access.   
	unsigned long	st_atime_nsec;
	long		st_mtime;	       Time of last modification.  
	unsigned long	st_mtime_nsec;
	long		st_ctime;	         Time of last status change.  

*/
  struct tm* tm = localtime(&s.st_atim.tv_sec);
//  printf( "%2d月  %2d %02d:%02d", tm->tm_mon + 1, tm->tm_mday,
//	  tm->tm_hour + 1,tm->tm_sec);


  
    printf("atime = %s",ctime(&s.st_atime));
    printf("mtime = %s",ctime(&s.st_mtime));
    printf("sctime = %s",ctime(&s.st_ctime));
    //使用宏函数检查文件属性
    if(S_ISDIR(s.st_mode))
        puts("文件夹");
    if(S_ISREG(s.st_mode))
        puts("普通文件");
 
}
