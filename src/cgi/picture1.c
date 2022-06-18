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

#define PHOTO_NUM_MAX		100 // 最多100张，0-99

#define DIRNAME		"../pice"
struct _syncmsg1
{
	int photo_num; // 上一次照片总数，最多100张，0-99
	int photo_idx; // 上一次浏览第几张，最多100张，0-99
	int key_action; // 这一次按键行为：上一张、下一张
};

struct _syncmsg2
{
	char number[12];
	char index[12];
	char action[12];
};


int cgiMain()
{
	DIR *dir;
	struct dirent *dirp;
	char photoname[PHOTO_NUM_MAX][48]; // 最多100张，0-99
	struct _syncmsg1 syncmsg1 = {0, 0, 0};     //cgi中操作的数据
	struct _syncmsg2 syncmsg2 = {{0}, {0}, {0}};//web页面传来数据
	char path[256] = {0};
	
	if((dir = opendir(DIRNAME)) == NULL)    //打开图片存放的目录
	{
		perror("fail to opendir");
		exit(1);
	}
	
	while((dirp = readdir(dir)) != NULL)  //读文件夹里文件的名字
	{
		if(dirp->d_name[0] > '9' || dirp->d_name[0] < '0')   //名字的第一个字母
			continue;
		sprintf(photoname[syncmsg1.photo_num++], "%s", dirp->d_name);
		if(syncmsg1.photo_num >= PHOTO_NUM_MAX)         //判断是否超过最大值
		{
			syncmsg1.photo_num = PHOTO_NUM_MAX - 1;
			break;
		}
	}
		
	syncmsg1.photo_num -= 1;

	cgiFormString("number", syncmsg2.number,13);      //网页传来的名字
	cgiFormString("index", syncmsg2.index,13);				//网页传来的指针
	cgiFormString("action", syncmsg2.action,13);			//网页传来的互动标志
	if(syncmsg2.number == NULL || syncmsg2.index == NULL || syncmsg2.action == NULL) //第一次浏览
	{	
		syncmsg1.photo_idx = 0; // 浏览第一张指针，最多100张，0-99
		syncmsg1.key_action = 0; // 浏览第一张行为，最多100张，0-99
	}
	else
	{
		syncmsg1.photo_idx = atoi(syncmsg2.index);   //把字符串转换成整型数
		if(syncmsg1.photo_idx > syncmsg1.photo_num)  // 
			syncmsg1.photo_idx = syncmsg1.photo_num;
//		if(syncmsg2.action[2] == 0x8A)       //prev
		if(syncmsg2.action[0] == 'p')
			syncmsg1.key_action = 1;		//进入上一张
//		else if(syncmsg2.action[2] == 0x8B) //next
		else if(syncmsg2.action[0] == 'n')
			syncmsg1.key_action = 2;	   //进入下一张
	}
	switch(syncmsg1.key_action)
	{
		case 0: // 浏览第一张行为
			sprintf(path, "%s/%s", DIRNAME, photoname[syncmsg1.photo_idx]);
			sprintf(syncmsg2.number, "%d", syncmsg1.photo_num);
			sprintf(syncmsg2.index, "%d", syncmsg1.photo_idx);
			break;
		case 1: // 浏览上一张行为
			if(syncmsg1.photo_idx >= syncmsg1.photo_num)
				syncmsg1.photo_idx = 0;
			else
				syncmsg1.photo_idx = syncmsg1.photo_idx + 1;
			sprintf(path, "%s/%s", DIRNAME, photoname[syncmsg1.photo_idx]);
			sprintf(syncmsg2.number, "%d", syncmsg1.photo_num);
			sprintf(syncmsg2.index, "%d", syncmsg1.photo_idx);
			break;
		case 2: // 浏览下一张行为
			if(syncmsg1.photo_idx <= 0)
				syncmsg1.photo_idx = syncmsg1.photo_num;
			else
				syncmsg1.photo_idx = syncmsg1.photo_idx - 1;
			sprintf(path, "%s/%s", DIRNAME, photoname[syncmsg1.photo_idx]);
			sprintf(syncmsg2.number, "%d", syncmsg1.photo_num);
			sprintf(syncmsg2.index, "%d", syncmsg1.photo_idx);
			break;
		default:
			break;	
	}

	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />");  
	fprintf(cgiOut, "<TITLE>历史照片信息</TITLE>");
	fprintf(cgiOut, "<style type=\"text/css\"> body,td,th {font-size: 12px;margin-left: 0px;margin-top: 0px;}</style></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#F8F9FA\">"); 

//	fprintf(cgiOut, "<H1>number:%d index:%d action:%d<Hi>",syncmsg1.photo_num,syncmsg1.photo_idx,syncmsg1.key_action);	
//	fprintf(cgiOut, "<H1>number:%s index:%s action:%s<Hi>",syncmsg2.number,syncmsg2.index,syncmsg2.action);	
	fprintf(cgiOut, "<img src=\"%s\" width=\"560\" height=\"420\" /></br>", path);

#if 1
	fprintf(cgiOut, "<table width=\"560\" border=\"0\" align=\"left\" cellpadding=\"0\" cellspacing=\"0\">");
	
	
	fprintf(cgiOut, 	"<form id=\"show_photo\" name=\"show_photo\" method=\"post\" action=\"./picture1.cgi\">");
	fprintf(cgiOut, 	"<tr>");
	
	
	fprintf(cgiOut, 		"<td height=\"30\" colspan=\"7\" align=\"center\"><input name=\"photoname\" style=\"background:transparent;border:1px solid #F8F9FA\" type=\"text\" id=\"photoname\" value=\"%s\" size=\"40\" maxlength=\"100\" readonly=\"readonly\"/></td>", photoname[syncmsg1.photo_idx]);
	fprintf(cgiOut, 	"</tr>");
	fprintf(cgiOut, 	"<tr>");
	
	
	fprintf(cgiOut, 		"<td height=\"30\" width=\"30%%\"><input style=\"display:none\" name=\"number\" type=\"text\" id=\"number\" value=\"%s\" size=\"10\" maxlength=\"100\"/></td>", syncmsg2.number);
	fprintf(cgiOut, 		"<td height=\"30\" width=\"15%%\" align=\"center\"><input type=\"submit\" name=\"action\" id=\"last\" value=\"prev\" /></td>"); // E4B88A E4B880 E5BCA0
	
	
	fprintf(cgiOut, 		"<td height=\"30\" width=\"10%%\"><input style=\"display:none\" name=\"index\" type=\"text\" id=\"index\" value=\"%s\" size=\"10\" maxlength=\"100\"/></td>", syncmsg2.index);
	fprintf(cgiOut, 		"<td height=\"30\" width=\"15%%\" align=\"center\"><input type=\"submit\" name=\"action\" id=\"next\" value=\"next\" /></td>"); // E4B88B E4B880 E5BCA0
	
	
	fprintf(cgiOut, 		"<td height=\"30\" width=\"30%%\"></td>");
	fprintf(cgiOut, 	"</tr>");
	
	fprintf(cgiOut, 	"</form>");
	
	
	fprintf(cgiOut, 	"</table>");
#endif
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);
	
	closedir(dir);
	
	return 0;
}
#if 0
int cgiMain()
{
	DIR *dir;
	struct dirent *dirp;

	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
	fprintf(cgiOut, " <html xmlns=\"http://www.w3.org/1999/xhtml\"><head>\n");
	fprintf(cgiOut, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	fprintf(cgiOut, "<link rel=\"stylesheet\" href=\"../images/style.css\" type=\"text/css\" />	<title>history Picture</title></head>\n");
	fprintf(cgiOut, "<body>\n");
	fprintf(cgiOut, "<H1 align=\"left\">history Picture:<H1>\n");
	fprintf(cgiOut, "<li><a href=\"/main1.html\">return to main</a></li>\n");
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
#endif
