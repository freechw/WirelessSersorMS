#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define QUEUE_MSG_LEN	256
#define PROJ_ID		'g'
#define PATH_NAME	"/app"
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
	int qid;
	int msglen;
	int i=0;
	struct msg msg_buf;

	/* get a message queue */
	key_t msgkey;
	if ((msgkey = ftok(PATH_NAME, PROJ_ID)) == -1) //�õ�ID��
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
		if (msgrcv(qid, &msg_buf, QUEUE_MSG_LEN, SERVER_MSG, 0) == -1) 
		{
			perror("Server msgrcv error!\n");
			exit(1);
		}
		printf("server rcv : %d: %s\n",msg_buf.msgtype,msg_buf.text);	
#if 0
		printf("client send: ");

		/*get string from terminal & fill up message data structure */
		fgets(msg_buf.text, QUEUE_MSG_LEN, stdin);
		if (strncmp("exit", msg_buf.text, 4) == 0) 
		{
			break;
		}
		msg_buf.text[strlen(msg_buf.text) - 1] = '\0';
		msg_buf.msgtype = CLIENT_MSG;
		msg_buf.num = i++;

		/*send message to message queue with CLIENT_MSG type */
		if (msgsnd(qid, &msg_buf, strlen(msg_buf.text) + 1+4, 0) == -1) 
		{
			perror("client msgsnd error!\n");
			exit(1);
		}
		#endif
	}
	exit(0);
}
