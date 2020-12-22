#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include<sys/wait.h> 
#include<stdlib.h> 

void wait_example()
{
	int status;
	if (fork() == 0)
		exit(1);
	else
		wait(&status);
	if (WIFEXITED(status))
		printf("Exit status: %d\n", WEXITSTATUS(status));
	else if(WIFSIGNALED(status))
		signal(WTERMSIG(status), (void *)"Exit signal");	
}

int		main()
{
	wait_example();
	return (0);	
}