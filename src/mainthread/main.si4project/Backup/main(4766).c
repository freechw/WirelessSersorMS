
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "link_list.h"


extern linklist envlinkHead, envlinkTail;


void show_list(linklist h)
{
	int i = 0;
	linklist p;
	
	p = h->next;
	while(p != NULL)
	{		
		printf("node[%d]:%s\n",i,p->data.text);
		i++;
		p = p->next;
	}
}


#if 1
void main()
{
	link_datatype data;
	linklist tmpnode;

	/*初始化链表*/
	envlinkHead = CreateEmptyLinklist();


	/*插入节点1*/
	memset (&data, 0, sizeof (link_datatype));

	data.msg_type = 0x1;
	memcpy(data.text,"1111",sizeof(data.text));

	if ((InsertLinknode (data)) == -1)
	{
		printf("NOMEM\n");
		return;
	}

	/*插入节点2*/
	memset (&data, 0, sizeof (link_datatype));

	data.msg_type = 0x2;
	memcpy(data.text,"2222",sizeof(data.text));

	if ((InsertLinknode (data)) == -1)
	{
		printf("NOMEM\n");
		return;
	}

		/*插入节点33*/
	memset (&data, 0, sizeof (link_datatype));

	data.msg_type = 0x3;
	memcpy(data.text,"3333",sizeof(data.text));

	if ((InsertLinknode (data)) == -1)
	{
		printf("NOMEM\n");
		return;
	}
	show_list(envlinkHead);
	printf("\n");
	/*提取节点*/
	while(1)
	{
		if ((tmpnode = GetLinknode(envlinkHead)) == NULL)
		{
			break;
		}
		show_list(envlinkHead);
		printf("\n");

		//printf("del type:%x \ntext:%s\n",tmpnode->data.msg_type,tmpnode->data.text);
		
		free (tmpnode);

	}
	return ;
}
#elif
int main()
{
	link_datatype data;
	linklist tmpnode;

	/*初始化链表*/
	envlinkHead = CreateEmptyCacheList();
	envlinkTail = envlinkHead;


	/*插入节点1*/
	memset (&data, 0, sizeof (link_datatype));

	data.msg_type = 0x1;
	memcpy(data.text,"1111",sizeof(data.text));

	if ((InsertCacheNode(&envlinkTail,data)) == -1)
	{
		printf("NOMEM\n");
		return -1;
	}

	/*插入节点2*/
	memset (&data, 0, sizeof (link_datatype));

	data.msg_type = 0x2;
	memcpy(data.text,"22222",sizeof(data.text));

	if ((InsertCacheNode(&envlinkTail,data)) == -1)
	{
		printf("NOMEM\n");
		return -1;
	}



	/*提取节点*/
	while(1)
	{
		if ((tmpnode = GetCacheNode(envlinkHead,&envlinkTail)) == NULL)
		{
			break;
		}

		printf("type:%x \ntext:%s\n",tmpnode->data.msg_type,tmpnode->data.text);
		
		free (tmpnode);

	}
	return 0;
}
#endif

