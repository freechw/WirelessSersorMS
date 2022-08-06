#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "data_global.h"

extern pthread_cond_t cond_zigbee_rcv;
extern pthread_cond_t cond_uart_cmd;
extern pthread_cond_t cond_main_thread;
extern pthread_cond_t cond_camera;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_refresh_updata;

extern pthread_mutex_t mutex_slinklist;
extern pthread_mutex_t mutex_zigbee_rcv;
extern pthread_mutex_t mutex_uart_cmd;
extern pthread_mutex_t mutex_main_thread;
extern pthread_mutex_t mutex_camera;
extern pthread_mutex_t mutex_refresh;
extern pthread_mutex_t mutex_refresh_updata;
extern pthread_mutex_t mutex_global;
extern pthread_mutex_t mutex_linklist;

extern int dev_camera_fd;
extern int dev_uart_fd;

extern int msgid;
extern int shmid;
extern int semid;


extern struct env_info_array all_info_RT; 

pthread_t 	id_zigbee_rcv,
			id_uart_cmd,
			id_main_thread,
			id_camera,
			id_refresh;

void ReleaseResource (int signo)
{

	pthread_mutex_destroy (&mutex_linklist);
	pthread_mutex_destroy (&mutex_global);
	pthread_mutex_destroy (&mutex_refresh_updata);
	pthread_mutex_destroy (&mutex_refresh);
	pthread_mutex_destroy (&mutex_camera);
	pthread_mutex_destroy (&mutex_main_thread);
	pthread_mutex_destroy (&mutex_uart_cmd);
	pthread_mutex_destroy (&mutex_zigbee_rcv);
	pthread_mutex_destroy (&mutex_slinklist);

	pthread_cond_destroy (&cond_refresh_updata);
	pthread_cond_destroy (&cond_refresh);
	pthread_cond_destroy (&cond_camera);
	pthread_cond_destroy (&cond_main_thread);
	pthread_cond_destroy (&cond_uart_cmd);
	pthread_cond_destroy (&cond_analysis);
	pthread_cond_destroy (&cond_sqlite);

	msgctl (msgid, IPC_RMID, NULL);
	shmctl (shmid, IPC_RMID, NULL);
//	semctl (semid, 1, IPC_RMID, NULL);

	pthread_cancel (id_refresh);
	pthread_cancel (id_camera);
	pthread_cancel (id_main_thread);
	pthread_cancel (id_uart_cmd);
	pthread_cancel (id_zigbee_rcv);

	close (dev_camera_fd);
	close (dev_uart_fd);

	printf ("All quit\n");

	exit(0);
}

void setLimit (int ep_no, float temMAX, float temMIN, float humMAX, float humMIN, float illMAX, float illMIN)
{
	if (ep_no >= 0 && (ep_no <=ENDPOIT_NUM - 1))
	{
		all_info_RT.env_no[ep_no].temperatureMAX = temMAX;
		all_info_RT.env_no[ep_no].temperatureMIN = temMIN;
		all_info_RT.env_no[ep_no].humidityMAX = humMAX;
		all_info_RT.env_no[ep_no].humidityMIN = humMIN;
		all_info_RT.env_no[ep_no].illuminationMAX = illMAX;
		all_info_RT.env_no[ep_no].illuminationMIN = illMIN;
	}
}

int main(int argc, char **argv)
{
#if 0
	Create_table ();  //创建环境表

	setLimit (1, 50, 5, 50, 5, 4000, 15);
	setLimit (2, 50, 5, 50, 5, 4000, 15);
#endif

	pthread_mutex_init (&mutex_slinklist, NULL);
	pthread_mutex_init (&mutex_uart_cmd, NULL);
	pthread_mutex_init (&mutex_main_thread, NULL);
	pthread_mutex_init (&mutex_camera, NULL);
	pthread_mutex_init (&mutex_refresh, NULL);
	pthread_mutex_init (&mutex_refresh_updata, NULL);
	pthread_mutex_init (&mutex_global, NULL);
	pthread_mutex_init (&mutex_linklist, NULL);

	pthread_cond_init (&cond_zigbee_rcv, NULL);
	pthread_cond_init (&cond_uart_cmd, NULL);
	pthread_cond_init (&cond_main_thread, NULL);
	pthread_cond_init (&cond_camera, NULL);
	pthread_cond_init (&cond_refresh, NULL);
	pthread_cond_init (&cond_refresh_updata, NULL);

	signal (SIGINT, ReleaseResource);

	pthread_create (&id_zigbee_rcv, 0, pthread_zigbee_rcv, NULL);
	sleep (1);
	pthread_create (&id_uart_cmd, 0, pthread_uart_send, NULL);
	pthread_create (&id_main_thread, 0, pthread_main, NULL);
	pthread_create (&id_camera, 0, pthread_camera, NULL);
	pthread_create (&id_refresh, 0, pthread_refresh, NULL);

	pthread_join (id_zigbee_rcv, NULL);
	printf ("g1\n");
	pthread_join (id_uart_cmd, NULL);
	printf ("g2\n");
	pthread_join (id_main_thread, NULL);
	printf ("g3\n");
	pthread_join (id_camera, NULL);
	printf ("g4\n");
	pthread_join (id_refresh, NULL);
	printf ("g5\n");

	return 0;
}
