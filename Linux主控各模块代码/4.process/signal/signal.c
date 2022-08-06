/*
 * (C) Copyright 2022
 * https://gitee.com/yikoulinux
 * 
 * This program is distributed in the purpose for training only
 * and hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Revision history
 * ------------------------------------------------------
 * 20120501	Daniel.Peng	initial 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

void sig_handler(int signo);

int main(int argc, char *argv[])
{
	int time_left;
	
	#if 1
	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		perror("signal error");
		return -1;
	}
	signal(SIGQUIT, sig_handler);	
	#endif
	printf("begin sleep ...\n");
	time_left = sleep(10);
	printf("end sleep ... time_left=%d\n", time_left);
	while(1);
	return 0;
}

void sig_handler(int signo)
{
	if (signo == SIGINT)
		printf("I received a SIGINT!\n");
	if (signo == SIGQUIT)
		printf("I received a SIGQUIT!\n");	
}
