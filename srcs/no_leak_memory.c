/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_leak_memory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 12:53:08 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/25 17:06:16 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** void	ft_free_2dem_arr(void ***arr)
** void	ft_free_arr(void **array)
** free tables after using malloc
*/

void	ft_free_arr(void **array)
{
	if (*array)
		free(*array);
	*array = NULL;
}

void	ft_free_2dem_arr(void ***arr)
{
	int	i;

	i = 0;
	while ((*arr)[i])
	{
		free((*arr)[i]);
		(*arr)[i] = NULL;
		i++;
	}
	free(*arr);
	*arr = NULL;
}

/*
** int ft_print__malloc(char *s1, char *s2, int n)
** print in stderr the msg of malloc failed and return the param 'n'
*/

int ft_print__malloc(char *s1, char *s2, int n)
{
	if (s1)
		ft_free_arr((void**)&s1);
	if (s2)
		ft_free_arr((void**)&s2);
	ft_putendl_fd("minishell: Error: failed to allocation memory.", 2);
	return (n);
}
