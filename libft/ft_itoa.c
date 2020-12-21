/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 10:54:53 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/02 10:40:31 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nlen(int n)
{
	int i;

	i = 0;
	if (n < 0)
	{
		n = n * -1;
		i++;
	}
	else if (n == 0)
		i++;
	while (n)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static char	*ft_putnbr(long int nbr, char *s, int len)
{
	int	j;
	int	tmp;

	tmp = len;
	j = len - 1;
	while (len)
	{
		if (nbr < 0)
		{
			nbr = nbr * -1;
			s[0] = '-';
			len--;
		}
		if (nbr >= 0)
		{
			s[j] = (nbr % 10) + '0';
			nbr = nbr / 10;
			j--;
			len--;
		}
	}
	s[tmp] = '\0';
	return (s);
}

char		*ft_itoa(int n)
{
	long int	nbr;
	char		*res;
	int			len;

	nbr = (long int)n;
	len = ft_nlen(nbr);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	ft_putnbr(nbr, res, len);
	return (res);
}
