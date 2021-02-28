/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 09:42:53 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/26 12:40:25 by asaadi           ###   ########.fr       */
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
# define PATH_MAX 1024
# include "parser.h"
# include "exec.h"

/*
** *********************** Parser *************************
*/

// typedef struct	s_cmd
// {
// 	t_list			*word_list;
// 	t_list			*redir_list;
// 	t_list			*subshell;
// 	struct s_cmd	*next;
// }				t_cmd;

// enum	e_state
// {
// 	e_state_nsc,
// 	e_state_sc,
// 	e_state_and,
// 	e_state_dand,
// 	e_state_pipe,
// 	e_state_dollar,
// 	e_state_dpipe,
// 	e_state_squote,
// 	e_state_dquote,
// 	e_state_scolon,
// 	e_state_gt,
// 	e_state_dgt,
// 	e_state_lt,
// 	e_state_dlt,
// 	e_state_openparen,
// 	e_state_closeparen,
// 	e_state_escape,
// 	e_state_delim,
// 	e_state_wildcard,
// 	e_state_wspace
// };

// typedef struct	s_redir
// {
// 	enum e_state	type;
// 	char 			*file;
// }				t_redir;


// typedef struct	s_token
// {
// 	enum e_state type;
// 	char *value;
// }				t_token;

// typedef struct	s_pipe
// {
// 	enum e_state condition;
// 	t_list *cmd_list;
// }				t_pipe;

// typedef struct	s_cond
// {
// 	int is_pipe;
// 	t_list *pipe_list;
// }				t_cond;

// t_list		*ft_tokenizer(char *str);
// void		quotes(t_list *tokens_list);
// void		subs_dollar(t_list *tl, char **env);
// void		dollar(t_list *tl, char **env);
// int			remove_token_by_type(t_list **tokens_list, enum e_state type);
// void		join_same_type(t_list *tokens_list, enum e_state type);
// t_list  	*fill_cmd(t_list *tl, t_cmd **cmd);
// t_list		*fill_cond(t_list *tokens_list, t_cond **cond);
// t_list		*fill_pipe(t_list *tokens_list, t_pipe **pipe, enum e_state condition);
// int			match(char *pattern, char *string, int p, int s);
// char		**get_dir_arr();
// void		free_dir_arr(char **dir_arr);
// void		create_pattern(t_list *tl);
// void		wildcard(t_list **tl);
// void		parse(t_list *tokens_list);

/*
** ***************** Execution functions ******************
*/

# define IS_DIR 100
# define IS_FILE 101



void		execution_part(t_pipe *pipe, t_exec *exec);

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
int 		ft_close_dup2_fds(int fd0, int fd1, t_exec *exec);
void		ft_free_2dem_arr(void ***arr);
void		ft_free_arr(void **arr);

char		*get_var_env(char **envp, char *var_to_check);
int			count_vars_env(char **env_list);
char		**envp_cpy(char **env);
void		cmds_execution(t_exec *exec, int pipe);
char		**fill_args(t_list *list_words);
int			exec_cmd(t_exec *exec);

#endif
