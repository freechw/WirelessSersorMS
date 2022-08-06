/**********************************************************************************************
 $$$$$			 $$$$			$$$$$			$$$$$$$$			 $$$$$		  $$$$$$		
 $$$$$$ 		$$$$$$		   $$$$$$		$$$$$$$$$$$$$$$ 		$$$$$$		  $$$$$$		
  $$$$$$	   $$$$$$$		  $$$$$$	  $$$$$$$$$$$$$$$$$ 	   $$$$$$$$ 	 $$$$$$$$		
   $$$$$	  $$$$$$$$$ 	  $$$$$ 	 $$$$$$ 				   $$$$$$$$ 	 $$$$$$$$		
	$$$$$	 $$$$$$$$$$ 	 $$$$$		 $$$$$$$$$$$$$			  $$$$$$$$$$	$$$$$$$$$		
	$$$$$$	 $$$$$ $$$$ 	$$$$$		 $$$$$$$$$$$$$$$$$		  $$$$$$$$$$   $$$$$ $$$$$		
	 $$$$$	$$$$$  $$$$$   $$$$$		   $$$$$$$$$$$$$$$$$	 $$$$$$$$$$$$ $$$$$$ $$$$$		
	  $$$$ $$$$$$  $$$$$  $$$$$ 					 $$$$$$$$	 $$$$$	$$$$$ $$$$$  $$$$$		
	  $$$$$$$$$$	$$$$$$$$$$		   $$$				$$$$$	$$$$$	$$$$$$$$$$	  $$$$$ 	 
	   $$$$$$$		$$$$$$$$$		   $$$$$$		$$$$$$$$   $$$$$$	 $$$$$$$$	  $$$$$$   
	   $$$$$$$		 $$$$$$$		   $$$$$$$$$$$$$$$$$$$$    $$$$$	  $$$$$$$	   $$$$$$   
		$$$$$		 $$$$$$ 			 $$$$$$$$$$$$$$$$	   $$$$$	  $$$$$$	   $$$$$$	
                                                                                          
 * Copyright (C) 2022 - 2023, HaoQing, <970407688@qq.com>.
 * 		<https://github.com/wuxiaolie/WirelessSersorMS>   
 * 		<https://gitee.com/yang-haoqing/wireless-sersor-ms>
 *
 * This software is licensed as described in the file COPYING, which you should have received 
 * as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell copies of 
 * the Software, and permit persons to whom the Software is furnished to do so, under 
 * the terms of the COPYING file.
 *
 *********************************************************************************************/

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
