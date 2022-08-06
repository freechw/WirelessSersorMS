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

enum{
	DEVICE_RELY = 0,
	DEVICE_BEEP,
	DEVICE_LED,
	DEVICE_MAX
};

UINT8 born_cmd(UINT8 ep_no,UINT8 device,UINT8 data)
{
	return ((ep_no - 48)) << 6 | (device << 4) | (data << 0);
}

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

	buf[0] = '1';
	ep_no[0] = '2';
/*  |--|--|----|
     no  dev  op
     10   10  0001
     
*/
	msg_buf.text[0] = born_cmd(ep_no[0],DEVICE_LED,(buf[0]-48));


	msg_buf.type = 1L;
	msg_buf.msgtype = 4L;
	msgsnd(msgid, &msg_buf,sizeof(msg_buf)-sizeof(long),0);




	buf[0] = '1';
	ep_no[0] = '1';
	msg_buf.text[0] = born_cmd(ep_no[0],DEVICE_BEEP,(buf[0]-48));


	msg_buf.type = 1L;
	msg_buf.msgtype = 4L;
	msgsnd(msgid, &msg_buf,sizeof(msg_buf)-sizeof(long),0);

	sleep(4);

	buf[0] = '0';
	ep_no[0] = '1';

	msg_buf.text[0] = born_cmd(ep_no[0],DEVICE_BEEP,(buf[0]-48));


	msg_buf.type = 1L;
	msg_buf.msgtype = 4L;
	msgsnd(msgid, &msg_buf,sizeof(msg_buf)-sizeof(long),0);

	sleep(4);

	buf[0] = '0';
	ep_no[0] = '1';

	msg_buf.text[0] = born_cmd(ep_no[0],DEVICE_BEEP,(buf[0]-48));


	msg_buf.type = 1L;
	msg_buf.msgtype = 4L;
	msgsnd(msgid, &msg_buf,sizeof(msg_buf)-sizeof(long),0);



	return 0; 
} 
