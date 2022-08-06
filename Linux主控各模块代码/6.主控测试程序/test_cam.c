#include <stdio.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define N 32


#define UINT8 unsigned char 

struct msg
{
	long type;
	long msgtype;
	unsigned char text[N];
};


int main() 
{ 
	key_t key;
	char buf[N];
	char ep_no[2];
	int msgid;
	struct msg msg_buf;
	memset(&msg_buf,0,sizeof(msg_buf));
	
	if((key = ftok("/app", 'g')) < 0)
	{
		perror("ftok");
		exit(1);
	}

	if((msgid = msgget(key, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}

	bzero (msg_buf.text, sizeof (msg_buf.text));

	msg_buf.type = 1L; //给主控程序的 pthread_main
	msg_buf.msgtype = 3L; //发送给摄像头 线程
	msg_buf.text[0] = 1;//拍照数量	
	msgsnd(msgid, &msg_buf,sizeof(msg_buf)-sizeof(long),0);

	return 0; 
} 
