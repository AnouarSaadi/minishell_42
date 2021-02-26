/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 10:40:11 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/26 11:35:44 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

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
# include "minishell.h"
# include "exec.h"

/*
** qsm: question mark.
*/
enum    e_state
{
	e_state_nsc,
	e_state_sc,
	e_state_and,
	e_state_dand,
	e_state_pipe,
	e_state_dollar,
	e_state_dpipe,
	e_state_squote,
	e_state_dquote,
	e_state_scolon,
	e_state_gt,
	e_state_dgt,
	e_state_lt,
	e_state_dlt,
	e_state_openparen,
	e_state_closeparen,
	e_state_escape,
	e_state_delim,
	e_state_wildcard,
	e_state_wspace,
	e_state_qsm,
	e_state_afterdollar,
	e_state_afterdollarqsm,
};

typedef struct	s_redir
{
	enum e_state	type;
	char 			*file;
}				t_redir;


typedef struct	s_token
{
	enum e_state type;
	char *value;
}				t_token;

typedef struct	s_pipe
{
	enum e_state condition;
	t_list *cmd_list;
}				t_pipe;

typedef struct	s_cond
{
	int is_pipe;
	t_list *pipe_list;
}				t_cond;


// typedef struct	s_exec
// {
// 	char	**envp;
// 	char	**args;
// 	int		index;
// 	int		code_ret; // This var will be used in $?
// }				t_exec;

t_list		*ft_tokenizer(char *str);
void		quotes(t_list *tokens_list);
void		subs_dollar(t_list *tl, char **env, int code_ret);
t_list		*remove_token_by_type(t_list **tokens_list, enum e_state type,
	   	enum e_state d);
void		join_same_type(t_list *tokens_list, enum e_state type, enum e_state d);
t_list  	*fill_cmd(t_list *tl, t_cmd **cmd);
void		ft_free_split(char **split);
char 		*get_var_env(char **envp, char *var_to_check);
int			match(char *pattern, char *string, int p, int s);
char		**get_dir_arr();
void		free_dir_arr(char **dir_arr);
t_list		*matched_dir_list(char **dir_arr, char *pattern);
t_list		*matched_dir_list_test(char *pattern);
t_token		*create_token(char *value, enum e_state type);
char		*change_to_one(char *str, char c);
t_list		*ft_tokenizer(char *str);
void		quotes(t_list *tokens_list);
// void		subs_dollar(t_list *tl, char **env);
void		dollar(t_list *tl, char **env);
// int			remove_token_by_type(t_list **tokens_list, enum e_state type);
// void		join_same_type(t_list *tokens_list, enum e_state type);
t_list  	*fill_cmd(t_list *tl, t_cmd **cmd);
// t_list		*fill_cond(t_list *tokens_list, t_cond **cond);
t_list		*fill_pipe(t_list *tokens_list, t_pipe **pipe);
int			match(char *pattern, char *string, int p, int s);
char		**get_dir_arr();
void		free_dir_arr(char **dir_arr);
void		create_pattern(t_list *tl);
void		wildcard(t_list **tl);
void		switch_state(t_list *tl, enum e_state from, enum e_state to);
void		parse(t_list **tokens_list, char **env, int *code_ret, t_exec *exec);

#endif