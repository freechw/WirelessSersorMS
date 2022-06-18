char buf[10];   //用于存放从管道读取的命令

int flags = 0;   //拍照标志，1：表示11张照片，2：表示1张照片

int fd_com = 0; //打开管道的文件描述符

 

   stop_num = 0; //拍照计数

  if ( access(“/tmp/webcom”,F_OK) < 0 )    //创建有名管道用于接收拍照命令

{

   if ( mkfifo(“/tmp/webcom”,0666 ) < 0)

   {

    Printf(“ photo fifo create failed\n”);

}

}

  fd_com = open (“/tmp/webcom”,O_RDONLY,0666);

  if (fd_com < 0)

{

    perror (“open the file webcom error”);

}

 

在while( ok >= 0 && !pglobal->stop){ 后加入

  if (flags == 0)

{

    while(1)

 {

read(fd_com,buf,sizeof(buf));

if (strncmp(buf,”danger”,6) == 0)    //拍11张照片

{

    flags = 1;

    bzero(buf,sizeof(buf));

    break;

}

if (strncmp(buf,”one”,3) == 0)   //拍1张照片

{

    flags = 2;

    bzero(buf,sizeof(buf));

    break;

}

 

}

}
