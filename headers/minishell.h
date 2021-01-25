/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 09:42:53 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/25 17:04:15 by asaadi           ###   ########.fr       */
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

/*
** *********************** Parser ************************* 
*/

typedef struct	s_cmd
{
	t_list			*word_list;
	t_list			*redir_list;
	struct s_cmd	*next;
}				t_cmd;

enum	e_state
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
	e_state_wspace
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

t_list		*ft_tokenizer(char *str);
void		quotes(t_list *tokens_list);
void		subs_dollar(t_list *tl, char **env);
int			remove_token_by_type(t_list **tokens_list, enum e_state type);
void		join_same_type(t_list *tokens_list, enum e_state type);
t_cmd		*fill_cmd(t_list *tl);

/*
** ***************** Built_ins functions ******************
*/

void		change_directory(char *_path, char **envp);
void		pwd_function(void);
void		echo_function(char *arg_to_print,int fd ,int newline);
void		env_function(char **envp);
void		export_function(char ***envp, char *var_to_add);
char		**unset_function(char **envp, char *var_to_remove);
void		exit_function(int _id);
char		*get_var_env(char **envp, char *var_to_check);
void		ft_free_2dem_arr(char **arr);
void		get_cmd_path(char **args, char **envp);
void		ft_free_arr(void *arr);
int			count_vars_env(char **env_list);
void		sort_print_envp_alpha(char **envp);
char		**envp_cpy(char **env);
int			ft_strcmp(const char *s1, const char *s2);
void print_envp(char **envp);



#endif