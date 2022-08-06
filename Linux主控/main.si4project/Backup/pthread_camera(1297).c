/*
 * 无线传感网综合项目主控程序.
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

#include "data_global.h"

extern char dev_camera_mask;
extern int dev_camera_fd;

extern pthread_mutex_t mutex_camera;

extern pthread_cond_t cond_camera;

void *pthread_camera (void *arg)
{
	unsigned char picture = 0;
#if 1
	if ((dev_camera_fd = open (DEV_CAMERA, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
	{
		printf ("Cann't open file %s\n",DEV_CAMERA);
	//	exit (-1);
	}
	printf ("pthread_camera is ok\n");
#endif
	while (1)	
	{
		pthread_mutex_lock (&mutex_camera);
		pthread_cond_wait (&cond_camera, &mutex_camera);
		picture = dev_camera_mask;
		pthread_mutex_unlock (&mutex_camera);
#if 1
		for (; picture > 0; picture--)
		{
			//write (dev_camera_fd, "one", 3); //无效
			system("echo one > /tmp/webcom");  //ok
			printf("picture = %d\n", picture);
			sleep(8);
		}
#endif
	}
}
