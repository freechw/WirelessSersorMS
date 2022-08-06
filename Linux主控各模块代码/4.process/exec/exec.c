#include <stdio.h>
#include <unistd.h>

int main()
{
	if (execlp("ps", "ps", "-ef", NULL) < 0)
	{		
		perror("execlp error!");
	}
 	return 0;
}