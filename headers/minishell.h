/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 09:42:53 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/03 17:44:24 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include "libft.h"
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <dirent.h>
# include <sys/stat.h>
# include "parser.h"
# include "exec.h"
# include <limits.h>
# define PATH_MAX 1024

/*
** ***************** Execution functions ******************
*/

# define IS_DIR 100
# define IS_FILE 101

void		execution_part(t_pipe *pipe, t_exec *exec,
		t_list *tl, t_list *cond_list);

/*
** Builtins
*/

int			change_directory(char *path, t_exec *exec);
int			pwd_function(void);
int			echo_function(char **args);
int			env_function(char **envp);
int			export_function(t_exec *exec);
int			export_func_2(t_exec *exec, char *arg);
int			sort_print_envp_alpha(char **envp);
int			unset_function(t_exec *exec);
int			exit_func(t_exec *exec);
int			built_ins_execution(t_exec *exec);
int			check_if_built_in(char *cmd);

/*
** Search and launch the right executable
*/

int			get_cmd_binary_path(t_exec *exec);
char		*concat_path_cmd(char *pathname, char **cmd);
char		*check_pathvar(t_exec *exec);
int			check_if_file_or_directory(char *av);
int			check_if_executable(const char *filename);

/*
** Redirections
*/

int			redir_is_in_cmd(t_exec *exec, t_cmd *cmd, int pipe);

/*
** pipes
*/

void		pipe_execution(t_list *pipe_cmd_list, t_exec *exec);

/*
** Check Errors && Leaks
*/

int			ft_print__malloc(char *s1, char *s2, int n);
int			execve_failure(char *arg, char *err_msg);
void		ft_close_dup2_fds(int fd0, int fd1, t_exec *exec);
void		ft_free_2dem_arr(void ***arr);
void		ft_free_arr(void **arr);
void		print_msg__fail(char *err_msg, char *name, t_exec *exec);
int			print_error(char **bin, char *msg, t_exec *exec, int code);

/*
** Signals and Termcap
*/

void		sig_handler(int sig);
void		get_return_signals(t_exec *exec);
void		handling_ctrl_d(t_exec *exec, char **line);

char		*get_var_env(char **envp, char *var_to_check);
int			count_vars_env(char **env_list);
char		**envp_cpy(char **env);
void		cmds_execution(t_exec *exec, int pipe);
char		**fill_args(t_list *list_words);
int			exec_cmd(t_exec *exec);

#endif
