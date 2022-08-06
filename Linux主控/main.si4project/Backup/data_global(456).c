#include "data_global.h"
#include "uart_cache.h"
#include "sqlite_link_list.h"

//条件变量
pthread_cond_t cond_zigbee_rcv;
pthread_cond_t cond_uart_cmd;
pthread_cond_t cond_client_request;
pthread_cond_t cond_camera;
pthread_cond_t cond_refresh;
pthread_cond_t cond_refresh_updata;

//互斥锁
pthread_mutex_t mutex_slinklist;
pthread_mutex_t mutex_zigbee_rcv;
pthread_mutex_t mutex_uart_cmd;
pthread_mutex_t mutex_client_request;
pthread_mutex_t mutex_camera;
pthread_mutex_t mutex_refresh;
pthread_mutex_t mutex_refresh_updata;
pthread_mutex_t mutex_global;
pthread_mutex_t mutex_linklist;

unsigned char dev_sms_cmd;
unsigned char dev_infrared_mask;
unsigned char dev_buzzer_mask;
unsigned char dev_led_mask;
unsigned char dev_camera_mask;
unsigned char dev_uart_mask;

uart_cache_list zigbee_cache_head, zigbee_cache_tail;

int dev_camera_fd;
int dev_uart_fd;   //"ttyUSB0"

int msgid; //消息队列
int shmid; //共享内存
int semid; //信号量

char cgi_status;
char qt_status;


/**** sqlite module ****/
int storageNum;
int goodsKinds;

struct env_info_array env_info_clien_addr_s;
struct env_info_array all_info_RT; //用于对应共享内存中数据

slinklist slinkHead, slinkTail;

void sendMsgQueue (long type, unsigned char text)
{
	struct msg msgbuf;
	msgbuf.type = 1L;
	msgbuf.msgtype = type;
	msgbuf.text[0] = text;
	msgsnd (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 0);
}
