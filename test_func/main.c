#include <unistd.h>
#include <fcntl.h>
#include "minishell.h"


int		main(int ac, char *av[])
{
	int fd;
	pid_t p;
	char *text;

	fd = open(av[1], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IRWXU);
	p = fork();
	if (p == 0)
		text = "baby\n";
	else
		text = "mama\n";
	write(fd, text, ft_strlen(text));
	close(fd);
	return (0);	
}