#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int	glob = 6;
int main()
{
	int local;
	int pid;

	local = 88;
	
	printf("parent[pid = %d]: before fork, glob(&glob) = %d(%p), local(&local) = %d(%p)\n",
		getpid(), glob, &glob, local, &local );
	
	if((pid = fork()) < 0) {
		perror("fail to fork");
		return -1;
	}
	
	if(pid == 0) { /* child process */
		printf("child[pid = %d]: after fork, glob(&glob) = %d(%p), local(&local) = %d(%p)\n",
			getpid(), glob, &glob, local, &local );
		glob++;
		local++;	
		printf("child[pid = %d]: changed data after fork, glob(&glob) = %d(%p), local(&local) = %d(%p)\n",
			getpid(), glob, &glob, local, &local );
	}else { /* parent process */
		sleep(2);
		printf("parent[pid = %d]: after fork, glob(&glob) = %d(%p), local(&local) = %d(%p)\n",
			getpid(), glob, &glob, local, &local );
	}
	/* return euqal to exit(0), but exit may cause a compile warning
	 * due to main() is declared to return with an integter 
	 */
	
	return 0; 
}
