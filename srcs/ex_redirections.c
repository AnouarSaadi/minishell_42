/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:04:12 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/03 16:35:35 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** fucntion print in stderr the message of error if there's somthing\
** wrong at the redirection work.
*/

void		print_msg__fail(char *err_msg, char *name, t_exec *exec)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(err_msg, 2);
	exec->index = 1;
	exec->code_ret = 1;
}

/*
** int ft_close_dup2_fds(int fd0, int fd1, t_exec *exec)
** function of close and dup the file descriptors and
** check the failure of dup2 and close
*/

void		ft_close_dup2_fds(int fd0, int fd1, t_exec *exec)
{
	if (dup2(fd0, fd1) == -1)
		print_msg__fail(strerror(errno), "dup2", exec);
	if (close(fd0) == -1)
		print_msg__fail(strerror(errno), "close", exec);
}

/*
** void get_input_ouput(t_list *tmp__redir, t_exec *exec)
** function get the fds from redirections
*/

static void	get_input_ouput(t_list *tmp__redir, char *file, t_exec *exec)
{
	int fds[2];

	if (((t_redir *)tmp__redir->content)->type == e_state_gt)
	{
		if ((fds[1] = open(file, O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
			print_msg__fail(strerror(errno), file, exec);
		else
			ft_close_dup2_fds(fds[1], 1, exec);
	}
	if (((t_redir *)tmp__redir->content)->type == e_state_dgt)
	{
		if ((fds[1] = open(file, O_CREAT | O_RDWR | O_APPEND, 0666)) == -1)
			print_msg__fail(strerror(errno), file, exec);
		else
			ft_close_dup2_fds(fds[1], 1, exec);
	}
	if (((t_redir *)tmp__redir->content)->type == e_state_lt)
	{
		if ((fds[0] = open(file, O_RDONLY)) == -1)
			print_msg__fail(strerror(errno), file, exec);
		else
			ft_close_dup2_fds(fds[0], 0, exec);
	}
}

/*
** int redir_is_in_cmd(t_exec *exec, t_cmd *cmd, int pipe)
** function works if there's a redirection in the command.
** return the code_ret by thhe execution commmand
*/

int			redir_is_in_cmd(t_exec *exec, t_cmd *cmd, int pipe)
{
	t_list	*tmp__redir;
	int		save_fds[2];

	save_fds[0] = dup(0);
	save_fds[1] = dup(1);
	exec->index = 0;
	tmp__redir = cmd->redir_list;
	while (tmp__redir)
	{
		get_input_ouput(tmp__redir,
				((t_redir *)tmp__redir->content)->file, exec);
		tmp__redir = tmp__redir->next;
	}
	exec->args = fill_args(cmd->word_list);
	if (!exec->index)
		cmds_execution(exec, pipe);
	ft_free_2dem_arr((void***)&(exec->args));
	ft_close_dup2_fds(save_fds[0], 0, exec);
	ft_close_dup2_fds(save_fds[1], 1, exec);
	return (exec->code_ret);
}
