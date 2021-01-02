/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/02 10:10:43 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int 	count_vars_env(char **env_list)
{
	int count;

	count = 0;
	if (!env_list)
		return (0);
	while (env_list[count])
		count++;
	return (count);
}

// char		**unset_env(char *l, char **envp)
// {
// 	char **p;
// 	char **ret;
// 	char **split;
// 	int	len;
// 	int i;

// 	p = envp;
// 	len = count_line(p);
// 	if (!(ret = (char **)malloc(sizeof(char *) * (len - 1))))
// 		return (NULL);
// 	i = 0;
// 	while (i < len)
// 	{
// 		split = ft_split(p[i], '=');
// 		if (ft_strncmp(split[0], l, ft_strlen(l)) != 0)
// 		{
// 			ret[i] = p[i];
// 		}
// 		i++;
// 		ft_free_split(split);
// 	}
// 	return (ret);
// }

// char		**export_env(char *l, char **envp)
// {
// 	char **p;
// 	int len;

// 	p = envp;
// 	len = count_line(p);
// 	p[len] = l;
// 	p[len + 1] = NULL;
// 	return (p);
// }


char	*check_var_env(char **envp, char *var_to_check)
{
	int	count_vars;
	char *val_to_ret;
	char **equ_split;
	char *tmp;

	count_vars = 0;
	val_to_ret = ft_strdup("");
	while (envp[count_vars])
	{
		equ_split = ft_split(envp[count_vars], '=');
		if (ft_strncmp(equ_split[0], var_to_check, ft_strlen(equ_split[0])) == 0)
		{
			tmp = ft_strchr(envp[count_vars], '=');
			val_to_ret = ft_strdup(tmp + 1);
		}
		ft_free_split(equ_split);
		count_vars++;
	}
	return (val_to_ret);
}

int		main(int ac, char **av, char **envp)
{
	
	// int i = 0;
	// int len = 0;
	ac = 0;
	av[1] = "hhhhh";
	// char **p;
	// // p = envp;
	// // ft_putnbr_fd(len, 1);
	// // write(1, "\n", 1);
	// p = unset_env("mia",envp);
	// len = count_line(p);
	// while (i < len)
	// {
	// 	puts(p[i]);
	// 	i++;
	// }
	// char *str = ft_strchr("t=lllll", '=');
	// ft_putendl_fd(str + 1, 1);
	char *str = check_var_env(envp, "PWD");
	ft_putendl_fd(str, 1);
	return (0);
}

// int main()
// {
// 	char *str;
// 	char **srch;

// 	str = "mayvar=300";
// 	srch = ft_split(str, '=');
// 	int i = 0;
// 	while(i < 2)
// 	{
// 		printf("%s ==> %s\n", str, srch[i]);
// 		i++;
// 	}
// 	return (0);
// }