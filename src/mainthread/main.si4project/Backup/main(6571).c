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

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);

void run();
int fg=0;
pthread_t pid; //循环读取串口数据专用进程

int serial_fd;
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
	newtio.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	return 0;
}

uart_init()
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
}

/*
   deviceID : 
*/
#define DEV_ID_LED    0X1
#define DEV_ID_DELAY 0X2
#define DEV_ID_FULL   0xff  //all sensor send data
//0x02ff00
//0x020101
struct cmd{
	unsigned char enddeviceID;  //广播  填充OXFF
	unsigned char deviceID;
	unsigned char data;
};
struct temp_1820{
	unsigned char templ;
	unsigned char temph;	
};
struct data{

	unsigned char enddeviceID;
	unsigned char relays;
	unsigned char leds;
	unsigned char infrare;
	unsigned short gas;
	struct temp_1820 temp;
	unsigned short light;
	unsigned short reserve[2];
};

int Menu() 
{
	system("clear");
	int option;
	printf("\n\t\t************************************************\n");
	printf("\n\t\t**               ALARM SYSTERM                **\n");
	printf("\n\t\t**               1----FLOG ALARM              **\n");
	printf("\n\t\t**               2----LIGHT SENSITIVE         **\n");
	printf("\n\t\t**               3----TEMPERATURE             **\n");
	printf("\n\t\t**               4----LED                     **\n");
	printf("\n\t\t**               5----RELAY                   **\n");
	printf("\n\t\t**               6----INFRARED                **\n");
	printf("\n\t\t**               7----REAL-TIME DISPLAY       **\n");
	printf("\n\t\t**               8----CHANGE PWD              **\n");
	printf("\n\t\t**               0----EXIT                    **\n");
	printf("\n\t\t************************************************\n"); 
	while(1)
	{ 
		printf("Please choose what you want: ");
		scanf("%d",&option); 
		if(option<0||option>8)
			printf("\t\t    choose error!\n");
		else 
			break;
	}
	return option; 
}
/*
	打印温度
*/
void print_temp(struct temp_1820 temp)
{
	int flag = 0;
	unsigned int num;
	char wendu[8];
	unsigned char temh,teml;
	
	temh = temp.temph;
	teml =  temp.templ;
	num=teml*625;  
	
	if(flag==1)            //判断正负温度
	{
		wendu[0]='-';              //+0x2d 为变"-"ASCII码
	}
	else 
		wendu[0]='+';  
	
	if((temh/10%10==0)&&(temh/100==0))
		wendu[1]='0';
	else 
		wendu[1]=temh/10%10+0x30;

	wendu[2]=temh%10+0x30;
	wendu[3]='.';
	wendu[4]=num/1000+0x30;//忽略小数点后1位的数
	wendu[5]='\0';//结束位
	
	printf("   temp   :%s 度\n",wendu);
	return;
}
void *func_uart(void * arg)
{
	struct data data;
	int len = 0;
	struct cmd cmd;
	
	cmd.deviceID      = DEV_ID_FULL;//0xff
	cmd.enddeviceID   = 2;	
	while(1)
	{

		tcflush(serial_fd, TCIOFLUSH);
		write(serial_fd,&cmd,sizeof(cmd));		
		
		sleep(1);
		len = read(serial_fd,&data,sizeof(data));
		if(len!= sizeof(data))
		{
			continue;
		}

		printf("----------------------\n");
		printf("   enddevice id :%d \n",data.enddeviceID);
		printf("   relay     :%d \n",data.relays);
		printf("   led        :%d \n",data.leds);
		printf("   infrare   :%d \n",data.infrare);
		printf("   gas   :%d \n",data.gas);		
		print_temp(data.temp);	   //要休整一下    todo			
		printf("   light   :%d \n",data.light);			
		printf("----------------------\n");

		sleep(1);
		
	}
}

void sig_func(int signo)
{
	pthread_cancel(pid);

	signal(SIGINT,SIG_DFL);
}

void run()
{
	while(1)
	{		
		int x=Menu(); 
		switch(x) 
		{	
		case 1:
			break;		
		case 2:
			break; 
		case 3:
			break; 
		case 4:
			break; 
		case 5:
			break; 
        case 6:
			break; 
		case 7:
			//实时显示

			//press ctrl+c  can  stop read uart
			//再次按下 就会终止进程，
			signal(SIGINT,sig_func);
       		pthread_create(&pid, NULL,func_uart, NULL);

			getchar();
			getchar();
                    break;
			//修改密码
		case 8:
			break; 			
		case 0:
			printf("\n\t\t     exit!\n\n");
			close(serial_fd);
			exit(0);
		default:
			fg=1;break;
		 }
		 if(fg)
			 break;
	 }
}

int main() 
{
//	Login();
//	printf("\t\tLogin successful!\n");
//	system("pause");
	uart_init();
	run();
	return 0;
}


