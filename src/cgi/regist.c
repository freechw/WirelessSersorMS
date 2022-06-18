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
#include <stdlib.h>
#include <unistd.h>
#include "cgic.h"
#include "sqlite3.h"

int cgiMain()
{
	char userbuf[10], pwbuf[10]; 
	char sql[1024], **result, *errmsg;
	int nrow, ncolumn;
	sqlite3 *db;
	if(sqlite3_open("user.db",&db) != 0)
	{
		printf("open user.db failed!\n");
		exit(-1);
	}
	cgiFormString("username",userbuf,10);
	cgiFormString("userpass",pwbuf,10);

	sprintf(sql,"insert into user values('%s','%s');",userbuf,pwbuf);


	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != 0)
	{
		perror("sqlite3_exec()\n");
		cgiHeaderContentType("text/html");
		fprintf(cgiOut, "<HTML>\n");
		fprintf(cgiOut, "<HTML><HEAD>\n");
		fprintf(cgiOut, "<TITLE>logining</TITLE></HEAD>\n");
		fprintf(cgiOut, "<BODY>");
		fprintf(cgiOut, "<H1>%s<Hi>","username or password is not right,please try again");		
		fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../index.html\">");
	}
	else{
		cgiHeaderContentType("text/html");
//		fprintf(cgiOut,"<request.seCharcterEncoding(%s);>",buf);
		fprintf(cgiOut, "<HTML>\n");
		fprintf(cgiOut, "<HTML><HEAD>\n");
		fprintf(cgiOut, "<TITLE>regist</TITLE></HEAD>\n");
		fprintf(cgiOut, "<BODY>");
		fprintf(cgiOut, "<H1>%s<Hi>","regist successfully,skipping...");		
		fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../index.html\">");
	}
	return 0;
}
