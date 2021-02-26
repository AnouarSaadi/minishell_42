/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 11:33:40 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/26 12:27:52 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "libft.h"

typedef struct	s_exec
{
	char	**envp;
	char	**args;
	int		index;
	int		code_ret; // This var will be used in $?
}				t_exec;


typedef struct	s_cmd
{
	t_list			*word_list;
	t_list			*redir_list;
}               t_cmd;


typedef struct	s_pipe
{
	// enum e_state condition;
	t_list *cmd_list;
}				t_pipe;

#endif