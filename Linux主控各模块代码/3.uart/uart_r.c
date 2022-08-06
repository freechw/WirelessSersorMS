#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<pthread.h>
#include <signal.h>

#define size 10
int serial_fd;


int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	if(tcgetattr( fd,&oldtio)  !=  0) 
	{
		perror("SetupSerial 1");
		return -1;
	}
	bzero(&newtio, sizeof( newtio ));
	newtio.c_cflag  |=  CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;
	
	switch(nBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}
	
	switch(nEvent)
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':  
		newtio.c_cflag &= ~PARENB;
		break;
	}
	
	switch(nSpeed)
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
        cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}

	if(nStop == 1)
		newtio.c_cflag &=  ~CSTOPB;
	else if (nStop == 2)
		newtio.c_cflag |=  CSTOPB;
	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 24;
	tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	return 0;
}

void uart_init(void)
{
	int nset1,nset2;

	serial_fd = open( "/dev/ttyUSB0", O_RDWR);
	if(serial_fd == -1)
	{
		printf("open() error\n");
		exit(1);
	}
	nset1 = set_opt(serial_fd, 115200, 8, 'N', 1);
	if(nset2 == -1)
	{
		printf("set_opt() error\n");
		exit(1);
	}
	return;
}
struct getEnvMsg
{
	unsigned char tem[2];
	unsigned char hum[2];
	unsigned char ep_no;	
	unsigned char x;
	unsigned char y;
	unsigned char z;

	unsigned short hongwai;
	unsigned short lux;	 
	unsigned short rsv2;
	unsigned short gas;	 
	unsigned short rsv3;
	unsigned short adc;			

};
struct s_msg{
	char head[3];
	char type;
	struct getEnvMsg encmsg;
};

int main() 
{
	int i =0 ;
	int len;
	char buf[10]="1111#";
	char tbuf[10]={0};
	struct s_msg msg;
	
	uart_init();
	tcflush(serial_fd, TCIOFLUSH);
	while(1)
	{
		
		len = read(serial_fd,&msg,sizeof(msg));
		printf("\n-------%d------\n",i++);
		putchar(msg.head[0]);
		putchar(msg.head[1]);
		putchar(msg.head[2]);
		putchar(msg.type);
		putchar('\n');

		printf("tem:%d\n",msg.encmsg.tem[1]);
		printf("hum:%d\n",msg.encmsg.hum[1]);
		printf("ep_no:%d\n",msg.encmsg.ep_no);
		printf("hongwai:%d\n",msg.encmsg.hongwai);
		printf("lux:%d\n",msg.encmsg.lux);		
		printf("gas:%d\n",msg.encmsg.gas);
		sleep(3);
		system("clear");
	}
	
	return 0;
}


