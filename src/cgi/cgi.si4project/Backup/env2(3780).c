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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <unistd.h>
#include "sem.h"
#include "cgic.h"
#include "data_global.h"

#define N 32

#define STO_NO 2

char status[2][6] = {"Close", "Open"};
char fan_status[4][6] = {"Close", "One", "Two", "Three"};

struct shm_addr
{
	char cgi_status;
	char qt_status;
	struct env_info_array zg_status;
};

int cgiMain()
{
	key_t key;
	int shmid,semid;
	struct shm_addr *shm_buf;
	
	
	if((key = ftok("/app",'i')) <0)
	{
		perror("ftok");
		exit(1);
	}
	printf("key = %x\n",key);

	if((semid  = semget(key, 1, 0666)) < 0)
	{
		perror("semget");
		exit(1);
	}

	if((shmid = shmget(key, N, 0666 )) == -1)
	{
			perror("shmget");
			exit(1);
	}

	if((shm_buf = (struct shm_addr*)shmat(shmid, NULL, 0)) == (void*)-1 )
	{
		perror("shmat");
		exit(1);
	}

	sem_p(semid,0);

	cgiHeaderContentType("text/html");
//	fprintf(cgiOut, "<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type = \"text/css\"><!--body{line-height:50%}--></style></head>");
///	fprintf(cgiOut, "<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style><!--body{line-height:50%}--></style></head>");
	fprintf(cgiOut, "<head><meta http-equiv=\"refresh\" content=\"1\"><style><!--body{line-height:50%}--></style></head>");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<BODY bgcolor=\"#666666\">\n");
	fprintf(cgiOut, "<h1><font color=\"#FF0000\">Storage #%d:</font></H2>\n ", STO_NO);
	if (shm_buf->zg_status.env_no[STO_NO].storage_status == 1)
	{
		fprintf(cgiOut, "<h2><font face=\"Broadway\"><font color=\"#FFFAF0\">Real-time information environment</font></font></H2>\n ");	
		fprintf(cgiOut, "<h4>Temperature:\t%0.2f</H4>\n ", shm_buf->zg_status.env_no[STO_NO].temperature);
		fprintf(cgiOut, "<h4>Humidity:\t%0.2f</H4>\n ", shm_buf->zg_status.env_no[STO_NO].humidity);
		fprintf(cgiOut, "<h4>Illumination:\t%0.2f</H4>\n ", shm_buf->zg_status.env_no[STO_NO].illumination);
		fprintf(cgiOut, "<h4>Battery:\t%0.2f</H4>\n ", shm_buf->zg_status.env_no[STO_NO].battery);
		fprintf(cgiOut, "<h4>Adc:\t%0.2f</H4>\n ", shm_buf->zg_status.env_no[STO_NO].adc);
		fprintf(cgiOut, "<h4>X:\t%d</H4>\n ", shm_buf->zg_status.env_no[STO_NO].x);
		fprintf(cgiOut, "<h4>Y:\t%d</H4>\n ", shm_buf->zg_status.env_no[STO_NO].y);
		fprintf(cgiOut, "<h4>Z:\t%d</H4>\n ", shm_buf->zg_status.env_no[STO_NO].z);
		//fprintf(cgiOut, "<h4>Buzzer:\t%s</H4>\n ", status[shm_buf->zg_status.env_no[STO_NO].buzzer_status]);
		//fprintf(cgiOut, "<h4>Led:\t%s</H4>\n ", status[shm_buf->zg_status.env_no[STO_NO].led_status]);
		//fprintf(cgiOut, "<h4>Fan:\t%s</H4>\n ", fan_status[shm_buf->zg_status.env_no[STO_NO].fan_status]);
		fprintf(cgiOut, "<h4>Temperature\nMAX: %0.0f MIN: %0.0f</H4>\n ", shm_buf->zg_status.env_no[STO_NO].temperatureMAX, shm_buf->zg_status.env_no[STO_NO].temperatureMIN);
		fprintf(cgiOut, "<h4>Humidity\nMAX: %0.0f MIN: %0.0f</H4>\n ", shm_buf->zg_status.env_no[STO_NO].humidityMAX, shm_buf->zg_status.env_no[STO_NO].humidityMIN);
		fprintf(cgiOut, "<h4>Illumination\nMAX: %0.0f MIN: %0.0f</H4>\n ", shm_buf->zg_status.env_no[STO_NO].illuminationMAX, shm_buf->zg_status.env_no[STO_NO].illuminationMIN);
	}
	else
	{
		fprintf(cgiOut, "<h2><font face=\"Broadway\"><font color=\"#FFFAF0\">Close!</font></font></H2>\n ");	
	}
//	fprintf(cgiOut, "<h3>:</H3>\n ");	
	fprintf(cgiOut, "</BODY></HTML>\n");	
	sem_v (semid, 0);
	return 0;
}
