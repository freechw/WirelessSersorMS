#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define BUF_SIZE	256
#define PROJ_ID		'g'
#define PATH_NAME	"/app"
#define SERVER_MSG	1
#define CLIENT_MSG	2

int main(void)
{
	/*message data structure */
	struct mymsgbuf 
	{
		long msgtype;
		int num;		
		char ctrlstring[BUF_SIZE];
	} msgbuffer;

	int qid;
	int msglen;
	int i=0;

	/* get a message queue */
	key_t msgkey;
	if ((msgkey = ftok(PATH_NAME, PROJ_ID)) == -1) //µÃµ½ID£¬
	{
		perror("ftok error!\n");
		exit(1);
	}
	if ((qid = msgget(msgkey, IPC_CREAT | 0660)) == -1) 
	{
		perror("msgget error!\n");
		exit(1);
	}

	while (1) 
	{
		/*receive message from message queue with SERVER_MSG type */
		if (msgrcv(qid, &msgbuffer, BUF_SIZE, SERVER_MSG, 0) == -1) 
		{
			perror("Server msgrcv error!\n");
			exit(1);
		}
		printf("server rcv : %d: %s\n",msgbuffer.num,msgbuffer.ctrlstring);
		printf("client send: ");
#if 0
		/*get string from terminal & fill up message data structure */
		fgets(msgbuffer.ctrlstring, BUF_SIZE, stdin);
		if (strncmp("exit", msgbuffer.ctrlstring, 4) == 0) 
		{
			break;
		}
		msgbuffer.ctrlstring[strlen(msgbuffer.ctrlstring) - 1] = '\0';
		msgbuffer.msgtype = CLIENT_MSG;
		msgbuffer.num = i++;

		/*send message to message queue with CLIENT_MSG type */
		if (msgsnd(qid, &msgbuffer, strlen(msgbuffer.ctrlstring) + 1+4, 0) == -1) 
		{
			perror("client msgsnd error!\n");
			exit(1);
		}
		#endif
	}
	exit(0);
}
