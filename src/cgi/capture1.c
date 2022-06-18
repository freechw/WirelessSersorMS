/*
 * 无线传感网综合项目cgi程序.
 * 	Copyright (C) 2022  Daniel Peng
 *
 * 	This program is free software; you can redistribute it and/or
 * 	modify it under the terms of the GNU General Public License
 * 	as published by the Free Software Foundation; either version 2
 * 	of the License, or (at your option) any later version.
 *
 * 	This program is distributed in the hope that it will be useful,
 * 	but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 	GNU General Public License for more details.
 *
 *
 * Additional updates, Copyright:
 *	Daniel Peng <21689991@qq.com>
 */

#include <stdio.h> 
#include "cgic.h" 
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

struct msg
{
	long type;
	long msgtype;
	unsigned char text[N];
};


int cgiMain() 
{ 
	key_t key;
	char buf[N];
	int msgid;

	struct msg msg_buf;
	memset(&msg_buf,0,sizeof(msg_buf));
//	int camera_fd;
//	int len = buf[0] - 48;
	cgiFormString("mode",buf,N);
	if(buf[0] <= '0' || buf[0] > '9')
		goto err;	

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
	
//	msg_buf.text[0] = buf[0];
	msg_buf.text[0] = buf[0] - 48;

	msg_buf.type = 1L;
	msg_buf.msgtype = 3L;
	msgsnd(msgid, &msg_buf,sizeof(msg_buf)-sizeof(long),0);


	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../video.html\">");
	fprintf(cgiOut, "<H2>抓拍<font color=\"#FF0000\" size=\"+3\">成功！</font>本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<H2>您抓拍了%d张</H2>", buf[0] - '0');
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 20000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;

err:
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../video.html\">");
	fprintf(cgiOut, "<H2>抓拍<font color=\"#FF0000\" size=\"+3\">失败！</font>本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<H2>您抓拍了0张</H2>");
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 20000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;	
} 
