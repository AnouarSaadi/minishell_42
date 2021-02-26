/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 14:14:51 by abel-mak          #+#    #+#             */
/*   Updated: 2020/12/29 10:02:30 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFFER_SIZE 32
# include <unistd.h>
# include <stdlib.h>
# include "libft.h"

typedef struct s_glist	t_glist;
struct					s_glist
{
	int		fd;
	char	*savedstr;
	t_glist	*next;
};
int						fill_line(char **buff, char **line,
	t_glist **lst, t_glist **head);
int						get_next_line(int fd, char **line);
t_glist					*ladd(t_glist **head, t_glist *new);
t_glist					*lnew(char *savedstr, int fd);
int						ft_glstclear(char **line, t_glist **lst, t_glist **head,
	int reset);
t_glist					*loop_l(t_glist *head, int fd);

#endif
