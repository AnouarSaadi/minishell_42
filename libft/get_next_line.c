/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 16:46:21 by abel-mak          #+#    #+#             */
/*   Updated: 2021/01/07 11:33:29 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./get_next_line.h"

t_glist		*lnew(char *savedstr, int fd)
{
	t_glist	*lst;

	if ((lst = (t_glist*)malloc(sizeof(t_glist))) == NULL)
		return (NULL);
	lst->savedstr = savedstr;
	lst->fd = fd;
	lst->next = NULL;
	return (lst);
}

int			ft_glstclear(char **line, t_glist **lst, t_glist **head, int reset)
{
	t_glist *tmp;

	*line = (reset == 1) ? ft_strdup("") : *line;
	tmp = *head;
	if (*lst == *head)
	{
		*head = (*head)->next;
		free((*lst)->savedstr);
		free(*lst);
		*lst = NULL;
		return (0);
	}
	while (tmp)
	{
		if (tmp->next == *lst)
		{
			tmp->next = (*lst)->next;
			free((*lst)->savedstr);
			free(*lst);
			*lst = NULL;
			break ;
		}
		tmp = tmp->next;
	}
	return (0);
}

t_glist		*loop_l(t_glist *head, int fd)
{
	if (!head)
		return (NULL);
	while (head)
	{
		if (head->fd == fd)
			return (head);
		head = head->next;
	}
	return (NULL);
}

int			fill_line(char **buff, char **line, t_glist **lst, t_glist **head)
{
	char	*tmp;
	char	*endlineaddre;

	if (buff != NULL)
		free(*buff);
	if ((*lst)->savedstr == NULL)
		return (-1);
	if (ft_strlen((*lst)->savedstr) == 0)
		return (ft_glstclear(line, lst, head, 1));
	endlineaddre = ft_strchr((*lst)->savedstr, '\n');
	tmp = (*lst)->savedstr;
	if (endlineaddre != NULL)
	{
		(*lst)->savedstr = ft_strdup(endlineaddre + 1);
		*endlineaddre = '\0';
	}
	else
		(*lst)->savedstr = ft_strdup("");
	if ((*line = ft_strdup(tmp)) == NULL)
		return (-1);
	free(tmp);
	return ((endlineaddre == NULL) ? ft_glstclear(line, lst, head, 0) : 1);
}

/*
** ladd could've returned NULL if lnew failed malloc and returned NULL
** this why add (lst == NULL) condition to return -1
*/

int			get_next_line(int fd, char **line)
{
	char			*tmp;
	char			*buff;
	int				bytread;
	static t_glist	*hd;
	t_glist			*lst;

	if (read(fd, NULL, 0) < 0 || line == NULL || BUFFER_SIZE <= 0)
		return (-1);
	lst = loop_l(hd, fd);
	hd = (!lst) ? ladd(&hd, lnew(ft_strdup(""), fd)) : hd;
	lst = (lst == NULL) ? hd : lst;
	if (lst != NULL && ft_strchr(lst->savedstr, '\n') != NULL)
		return (fill_line(NULL, line, &lst, &hd));
	if (lst == NULL || (buff = (char*)malloc(BUFFER_SIZE + 1)) == NULL)
		return (-1);
	while ((bytread = read(fd, buff, BUFFER_SIZE)) > 0)
	{
		tmp = lst->savedstr;
		buff[bytread] = '\0';
		lst->savedstr = ft_strjoin(tmp, buff);
		free(tmp);
		if (ft_strchr(buff, '\n') != NULL || lst->savedstr == NULL)
			break ;
	}
	return (fill_line(&buff, line, &lst, &hd));
}
