#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>

#define QUEUE_MSG_LEN	256
#define PROJ_ID		32
#define PATH_NAME	"/tmp"
#define SERVER_MSG	1
#define CLIENT_MSG	2
/*message data structure */
struct msg 
{
	long type;
	long msgtype;
	unsigned char text[QUEUE_MSG_LEN];

};

int main(void)
{
	/*message data structure */
	struct msg msg_buf;


	int qid;
	int msglen;
	int i=0;
	/*get message queue */
	key_t msgkey;
	if ((msgkey = ftok(PATH_NAME, PROJ_ID)) == -1) 
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
		printf("server send: ");
		/*get string from terminal & fill up message data structure */
		fgets(msg_buf.text, QUEUE_MSG_LEN, stdin);
		if (strncmp("exit", msg_buf.text, 4) == 0) 
		{
			msgctl(qid, IPC_RMID, NULL);
			break;
		}
		msg_buf.text[strlen(msg_buf.text) - 1] = '\0';
		msg_buf.type = SERVER_MSG;
		msg_buf.msgtype = i++;

		/*send message to message queue with SERVER_MSG type */
		if (msgsnd(qid, &msg_buf, sizeof(struct msg) - sizeof(long), 0) == -1) 
		{
			perror("Server msgsnd error!\n");
			exit(1);
		}
#if 0
		/*receive message from message queue with CLIENT_MSG type */
		if (msgrcv(qid, &msg_buf, QUEUE_MSG_LEN, CLIENT_MSG, 0) == -1) 
		{
			perror("Server msgrcv error!\n");
			exit(1);
		}
		printf("server rcv: %d: %s\n",msg_buf.num,msg_buf.text);
#endif		
	}

	exit(0);
}
