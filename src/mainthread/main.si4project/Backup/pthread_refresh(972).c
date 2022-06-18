#include "sem.h"
#include "data_global.h"
#include "link_list.h"

#define N 1024

extern pthread_mutex_t mutex_refresh;
extern pthread_mutex_t mutex_refresh_updata;
extern pthread_mutex_t mutex_global;
extern pthread_mutex_t mutex_slinklist;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_refresh_updata;
extern pthread_mutex_t mutex_linklist;

extern char cgi_status;
extern char qt_status;
extern int shmid; 
extern int semid;
extern struct env_info_array all_info_RT;
extern linklist envlinkHead, envlinkTail;

struct shm_addr
{
	char cgi_status;
	char qt_status;
	struct env_info_array rt_status;
};


struct getEnvMsg
{
	unsigned char ep_no;
	unsigned char tem[2];
	unsigned char hum[2];
	unsigned char x;
	unsigned char y;
	unsigned char z;
	unsigned int ill;
	unsigned int battery;
	unsigned int adc;
};

float dota_adc (unsigned int ratio)
{
	return ((ratio * 3.3) / 1024);
}

void getEnvPackage (link_datatype *buf)
{
	struct getEnvMsg pack;
	float temh,teml;
	int ep_no;
	
	memcpy (&pack, buf->text, 20);
	ep_no = pack.ep_no;

	pthread_mutex_lock (&mutex_global);
	struct env_info current = all_info_RT.env_no[ep_no];
	pthread_mutex_unlock (&mutex_global);

	current.storage_status = 1;
	current.x = pack.x;//红外
	current.y = pack.y;
	current.z = pack.z;
	
	temh =  pack.tem[1];
	teml =  pack.tem[0];
	current.temperature = temh + teml*625/10000;
	current.illumination = pack.ill;//光强
	current.battery = dota_adc (pack.battery);//烟雾
//  current.adc = dota_adc (pack.adc);

	printf ("ep_no = %d tem = %0.2f hum = %0.2f ill = %0.2f battery = %0.2f adc = %0.2f x = %d y = %d z = %d\n", ep_no,
			current.temperature, current.humidity, current.illumination, current.battery, current.adc, current.x, current.y, current.z);

	//checkEnv (sto_no, &current);	

	pthread_mutex_lock (&mutex_global);
	all_info_RT.env_no[ep_no] = current;
	pthread_mutex_unlock (&mutex_global);



	return ;
}

void *pthread_refresh (void *arg)
{
	key_t key_info;
	int shmid, semid;
	linklist node;
	link_datatype buf;

	struct shm_addr *shm_buf;

	if ((key_info = ftok ("/app", 'i')) < 0)
	{
		perror ("ftok info");
		exit (-1);
	}

	if ((semid = semget (key_info, 1, IPC_CREAT | IPC_EXCL |0666)) < 0)
	{
		if (errno == EEXIST)
		{
			semid = semget (key_info, 1, 0666);
		}
		else
		{
			perror ("semget");
			exit (-1);
		}
	}
	else
	{
		init_sem (semid, 0, 1);
	}

	if ((shmid = shmget (key_info, N, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		if (errno == EEXIST)
		{
			shmid = shmget (key_info, N, 0666);
			shm_buf = (struct shm_addr *)shmat (shmid, NULL, 0);
			
		}
		else
		{
			perror ("shmget");
			exit (-1);
		}

	}
	else
	{
		if ((shm_buf = (struct shm_addr *)shmat (shmid, NULL, 0)) == (void *)-1)
		{
			perror ("shmat");
			exit (-1);
		}
	}

	bzero (shm_buf, sizeof (struct shm_addr));
	printf ("pthread_refresh is ok\n");

	while (1)
	{
		pthread_mutex_lock (&mutex_refresh);
		pthread_cond_wait (&cond_refresh, &mutex_refresh);
		while (1)
		{
			pthread_mutex_lock (&mutex_linklist);
			if ((node = GetLinknode (envlinkHead)) == NULL)
			{
				pthread_mutex_unlock (&mutex_linklist);
				break;
			}
			buf = node->data;
			free (node);
			pthread_mutex_unlock (&mutex_linklist);

			if ('e' == buf.msg_type)
			{
				getEnvPackage (&buf);
			}
		}
		//共享内存shm_buf需要由进程信号量来控制互斥访问
		sem_p (semid, 0);
		pthread_mutex_lock (&mutex_global);
		shm_buf->rt_status = all_info_RT;
		pthread_mutex_unlock (&mutex_global);
		sem_v (semid, 0);
		pthread_mutex_unlock (&mutex_refresh);
	}
	return 0;
}

