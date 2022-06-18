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
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iconv.h>
#include <sys/types.h>
#include <sys/wait.h>


int cgiMain()
{
	DIR *dir;
	struct dirent *dirp;

	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"https://gitee.com/yikoulinux\">\n");
	fprintf(cgiOut, " <html xmlns=\"https://gitee.com/yikoulinux\"><head>\n");
	fprintf(cgiOut, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	fprintf(cgiOut, "<link rel=\"stylesheet\" href=\"../images/style.css\" type=\"text/css\" />	<title>history Picture</title></head>\n");
	fprintf(cgiOut, "<body>\n");
	fprintf(cgiOut, "<H1 align=\"left\">history Picture:<H1>\n");
	fprintf(cgiOut, "<li><a href=\"/main2.html\">return to main</a></li>\n");
	fprintf(cgiOut, "<li><a href=\"/cgi-bin/delete.cgi\">delete all the pictures</a></li>\n");

	if((dir = opendir("../pice/")) == NULL)
	{
		perror("fail to opendir");
		return -1;
	}

	if(dir != NULL)
	{
		while((dirp = readdir(dir)) != NULL)
		{
			if(dirp->d_name[0] == '.') continue;
			fprintf(cgiOut, "<div align=\"center\">");
			fprintf(cgiOut,"%s",dirp->d_name);
			fprintf(cgiOut, "</div>");
			fprintf(cgiOut, "<div align=\"center\"><img src=\"../pice/%s\" width=\"320\" height=\"240\" />",dirp->d_name);
			fprintf(cgiOut, "</div>");
		}
	}

	fprintf(cgiOut, "</BODY></HTML>");
	

	return 0;
}
