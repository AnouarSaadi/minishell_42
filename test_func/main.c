#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

// void	display_prompt(void)
// {
// 	static int	first_time;
// 	const char	*clear_screen;

// 	first_time = 1;
// 	if (first_time)
// 	{
// 		clear_screen = " \e[1;1H\e[2J";
// 		write(1, clear_screen, 12);
// 		first_time = 0;
// 	}
// 	write(1, "#", 1);
// }

// int		main()
// {
// 	char	cmd[100];
// 	char	command[100];
// 	char	*parameter[100];
// 	char	*envp[] = {(char *) "PATH=/bin", 0};

// 	while (1)
// 	{
// 		display_prompt();
// 		// // read_command(command, parameter);
// 		// if (fork() != 0)
// 		// 	wait(NULL);
// 		// else
// 		// {
// 		// 	strcpy(cmd, "/bin/");
// 		// 	strcat(cmd, command);
// 		// 	execve(cmd, parameter, envp);
// 		// }
// 		// if (strcmp(command, "exit") == 0)
// 		// 	break;
// 		// l = 0;
// 	}
// 	return (0);
// }
int main()
{
	char *str;
	char **srch;

	str = "mayvar=300";
	srch = split(str, '=');
	int i = 0;
	while(i < 2)
	{
		printf("%s ==> %s\n", str, srch[i]);
		i++;
	}
	return (0);
}