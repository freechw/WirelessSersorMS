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

#include "cgic.h"
#include <stdlib.h>

int cgiMain()
{
	system("rm /www/pice/*");

	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<TITLE>logining</TITLE></HEAD>\n");
	fprintf(cgiOut, "<BODY>");
	fprintf(cgiOut, "<H1>%s<Hi>","delete pictures successfully,skipping...");		
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"0;url=/cgi-bin/picture1.cgi\">");
	return 0;
}
