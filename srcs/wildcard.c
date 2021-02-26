/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 11:09:32 by abel-mak          #+#    #+#             */
/*   Updated: 2021/02/25 16:01:15 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include <sys/stat.h>

int match(char *pattern, char *string, int p, int s)
{
	int ret;

	while (p < ft_strlen(pattern))
	{
		//&& s < ft_strlen(string))
		//'*' match '.' if it's not the first character
		if (pattern[p] == '*'&& (string[s] != '.' || s != 0))
		{
			if (s < ft_strlen(string) && (ret = match(pattern, string, p, s+1)))
				return ret;
			else
				return (match(pattern, string, p+1, s));
		}
		else if (s < ft_strlen(string) && pattern[p] == string[s])
		{ 
			p++;
			s++;
		}
		else
			return 0;
	}
	if (p == ft_strlen(pattern) && s == ft_strlen(string))
		return (1);
	else
		return (0);
}

size_t dirlen(char *name)
{
	DIR *d;
	struct dirent *dir;
	size_t len;

	len = 0;
	d = opendir(name);
	if (d) 
	{
		while ((dir = readdir(d)) != NULL)
		{
			len++;
		}
		closedir(d);
	}
	return (len);
}

void	swipe(char **str1, char **str2)
{
	char *tmp;

	tmp = *str1;
	*str1 = *str2;
	*str2 = tmp;
}

void sort_dir_arr(char **dir_arr)
{
	int i;
	size_t len;

	if (dir_arr[0] == NULL)
		return ;
	i = 1;
	while (dir_arr[i] != NULL)
	{
		len = (ft_strlen(dir_arr[i - 1]) > ft_strlen(dir_arr[i])) ?
			ft_strlen(dir_arr[i - 1]) : ft_strlen(dir_arr[i]);
		if (ft_strncmp(dir_arr[i - 1], dir_arr[i], len) > 0)
		{
			swipe(&dir_arr[i - 1], &dir_arr[i]);
			i = 0;
		}
		i++;
	}
}

/*
**
*/

//char **get_dir_arr()
//{
//	DIR *d;
//	struct dirent *dir;
//	size_t i;
//	char **dir_arr;
//
//	dir_arr = (char**)malloc(sizeof(char*) * (dirlen(".") + 1));
//	i = 0;
//	d = opendir(".");
//	while ((dir = readdir(d)) != NULL)
//	{
//		dir_arr[i] = ft_strdup(dir->d_name);
//		i++;
//	}
//	closedir(d);
//	dir_arr[i] = NULL;
//	sort_dir_arr(dir_arr, NULL);
//	return (dir_arr);
//}

/*
** ENOTDIR name is not a directory.
*/

int is_dir(char *dir_name, char *sub_dir_name)
{
	char	*full_dir;
	char	*tmp;
	struct stat statbuf;

	tmp = NULL;
	if (ft_strchr(dir_name, '/') == NULL)
	{
		tmp = ft_strjoin(dir_name, "/");
		full_dir = ft_strjoin(tmp, sub_dir_name);
	}
	else
		full_dir = ft_strjoin(dir_name, sub_dir_name);
	if (tmp != NULL)	
		free(tmp);
	free(full_dir);
	if (stat(full_dir, &statbuf) == -1 || S_ISDIR(statbuf.st_mode) == 0)
		return (0);
	return (1);
}

char	**fill_dir_arr(DIR *d, char *onlydir, char *dir_name, char *pattern)
{
	struct dirent	*dir;
	size_t 			i;
	char 			**dir_arr;

	dir_arr = (char**)malloc(sizeof(char*) * (dirlen(dir_name) + 1));
	i = 0;
	while (d != NULL && (dir = readdir(d)) != NULL)
	{
		if ((onlydir == NULL || is_dir(dir_name, dir->d_name) == 1) 
				&& match(pattern, dir->d_name, 0, 0) == 1)
		{
			dir_arr[i] = ft_strdup(dir->d_name);
			i++;
		}
	}
	dir_arr[i] = NULL;
	return (dir_arr);
}

/*
** onlydir is result of strchr of '/' on pattern NULL only dir must be returned
*/

char **get_dir_arr_test(char *dir_name, char *onlydir, char *pattern)
{
	DIR *d;
	char **dir_arr;

	pattern = ft_strdup(pattern);
	if (dir_name == NULL)
		dir_name = ".";
	d = opendir(dir_name);
	if (onlydir != NULL)
		*(ft_strchr(pattern, '/')) = '\0';
	dir_arr = fill_dir_arr(d, onlydir, dir_name, pattern);
	if (d != NULL)
		closedir(d);
//	sort_dir_arr(dir_arr, onlydir);
	free(pattern);
	return (dir_arr);
}


void free_dir_arr(char **dir_arr)
{
	int i;

	i = 0;
	while (dir_arr[i] != NULL)
	{
		free(dir_arr[i]);
		dir_arr[i] = NULL;
		i++;
	}
	free(dir_arr);
}

/*
 **
 */

//t_list *matched_dir_list(char **dir_arr, char *pattern)
//{
//	int i;
//	t_list *res;
//	char *simplifyed_pattern;
//
//	simplifyed_pattern = change_to_one(pattern, '*');
//	res = NULL;
//	i = 0;
//	while (dir_arr[i] != NULL)
//	{
//		if (match(simplifyed_pattern, dir_arr[i], 0, 0) == 1)
//		{
//			ft_lstadd_back(&res, 
//					ft_lstnew(create_token(ft_strdup(dir_arr[i]), e_state_nsc)));
//		}
//		i++;
//	}
//	free(simplifyed_pattern);
//	return (res);
//}


enum e_type
{
	e_path_path,
	e_path_pattern,
	e_path_array,
};

void	print_token(t_list *lst)
{
	while (lst != NULL)
	{
		printf(" |%s", ((t_token*)lst->content)->value);
		printf(" %d| ", ((t_token*)lst->content)->type);
		lst = lst->next;
	}
	printf("\n");
}

void	print_lst(t_list *lst)
{
	while (lst != NULL)
	{
		printf("%s", ((t_token*)lst->content)->value);
		printf(" %d\n", ((t_token*)lst->content)->type);
		lst = lst->next;
	}
}


t_list	*duplicate(t_list *path_tokens, char *dir_name)
{
	t_list	*tmp;
	t_list	*res;

	tmp = path_tokens;
	res = NULL;
	while (tmp != NULL)
	{
		if (((t_token*)tmp->content)->type == e_path_array)
			ft_lstadd_back(&res, ft_lstnew(create_token
						(ft_strdup(dir_name),(enum e_state)e_path_path)));
		else
			ft_lstadd_back(&res, ft_lstnew(
						create_token(ft_strdup(((t_token*)tmp->content)->value),
							(enum e_state)((t_token*)tmp->content)->type)));
		tmp = tmp->next;
	}
	return (res);
}

void    free_path_tokens(t_list *path_tokens);
int     path_exist(char *pathname);

t_list *get_path_list(t_list *path_tokens, char **dir_arr)
{
	int		i;
	t_list	*path_list;
	t_list	*dup;

	i = 0;
	path_list = NULL;
	sort_dir_arr(dir_arr);
	while (dir_arr[i] != NULL)
	{
		dup = duplicate(path_tokens, dir_arr[i]);
		join_same_type(dup, (enum e_state)e_path_path, 0);
		ft_lstadd_back(&path_list, ft_lstnew(dup));
		i++;
	}
	return (path_list);
}

/*
 ** so every path_list elem will contain path_tokens as content.
 ** if path_list content(path_tokens) contain token type pattern it means
 ** that it should be expanded to another path_list and so on...
 ** whenever we reach state where path_list has no more content containing tokens
 ** type pattern it's done this one should not be expanded
 **
 ** Procedure:
 ** step1: check if there is token type pattern
 ** step2: replace that token with e_path_array
 ** step3: move from e_path_array to e_path_path (duplicate)
 */

void	append_slash(char **arr, char *onlydir)
{
	int		i;
	char	*tmp;

	if (onlydir == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		tmp = arr[i];
		arr[i] = ft_strjoin(arr[i], "/");
		free(tmp);
		i++;
	}
}

/*
 ** if the array is not empty replace pattern with the array
 ** else if it's empty(no match) keep it and change type to e_state_path
 */

void	replace_token(t_list *path_tokens, t_token *token, char *pattern,
		char *onlydir)
{
	if (*((char**)token->value) != NULL)
	{
		append_slash((char**)token->value, onlydir);
		free(pattern);
		token->type = (enum e_state)e_path_array;
	}
	else
	{
		free((char**)token->value);
		token->value = pattern;
		(token)->type = (enum e_state)e_path_path;
	}
}

/*
 ** for the first elem if it's pattern
 */

void pattern_to_array_first(t_list *path_tokens)
{
	char *pattern;
	char *onlydir;

	if (path_tokens != NULL 
			&& ((t_token*)path_tokens->content)->type == e_path_pattern)
	{
		pattern = ((t_token*)path_tokens->content)->value;
		onlydir = ft_strchr(((t_token*)path_tokens->content)->value, '/');
		((t_token*)path_tokens->content)->value = (char*)
			get_dir_arr_test(".", onlydir,((t_token*)path_tokens->content)->value);
		replace_token(path_tokens, path_tokens->content, pattern, onlydir);
		join_same_type(path_tokens, (enum e_state)e_path_path, 0);
	}
}

/*
 ** get_dir_arr_test return char** and token->value takes char* so i cast it
 ** if onlydir join array elem with '/'
 */

void	pattern_to_array(t_list *path_tokens)
{
	t_list *next;
	char *pattern;
	char *onlydir;
	t_list	*tmp;

	tmp = path_tokens;
	pattern_to_array_first(tmp);
	while (tmp != NULL)
	{
		if (((t_token*)tmp->content)->type == e_path_array)
			break ;
		next = tmp->next;
		if (next != NULL && ((t_token*)next->content)->type == e_path_pattern)
		{
			pattern = ((t_token*)next->content)->value;
			onlydir = ft_strchr(pattern, '/');
			((t_token*)next->content)->value = (char*)
				get_dir_arr_test(((t_token*)tmp->content)->value, 
						onlydir,((t_token*)next->content)->value);
			replace_token(next, next->content, pattern, onlydir);
			join_same_type(path_tokens, (enum e_state)e_path_path, 0);
			break ;
		}
		tmp = tmp->next;
	}
}

t_list *split_path_tokens(char *str, t_list *tokens);

char	**get_arr(t_list *path_tokens)
{
	while (path_tokens != NULL)
	{
		if (((t_token*)path_tokens->content)->type == e_path_array)
			return ((char**)((t_token*)path_tokens->content)->value);
		path_tokens = path_tokens->next;
	}
	return (NULL);
}

void    free_token(t_list *elem);

void	free_path_tokens(t_list *path_tokens)
{
	int i;
	char **arr;

	if (path_tokens != NULL && path_tokens->next != NULL)
		free_path_tokens(path_tokens->next);
	if (path_tokens != NULL 
			&& ((t_token*)path_tokens->content)->type != e_path_array)
	{
		free_token(path_tokens);
	}
	else if (path_tokens != NULL)
	{
		arr = (char**)((t_token*)path_tokens->content)->value;
		i = 0;
		while (arr[i] != NULL)
		{
			free(arr[i]);
			i++;
		}
		free(arr);
		free((t_token*)path_tokens->content);
		free(path_tokens);
	}
}

void	replace_path_list(t_list **tmp, char **arr)
{
	t_list *to_remove;

	to_remove = *tmp;
	*tmp = get_path_list((*tmp)->content, arr);
	(ft_lstlast(*tmp))->next = to_remove->next;
	free_path_tokens(to_remove->content);
	free(to_remove);
}

void	expand_first(t_list **path_list)
{
	t_list *tmp;
	char	**arr;

	tmp = *path_list;
	while (tmp != NULL && (ft_lstsize(tmp->content) > 1 
				|| get_arr(tmp->content) != NULL))
	{
		pattern_to_array(tmp->content);
		arr = get_arr(tmp->content);
		if (arr != NULL)
			replace_path_list(&tmp, arr);
	}
	*path_list = tmp;
}

void	expand(t_list **path_list)
{
	t_list	*tmp;
	t_list	*tmp_path_tokens;
	char	**arr;

	expand_first(path_list);
	tmp = *path_list;
	while (tmp != NULL)
	{
		if (tmp->next != NULL && (ft_lstsize(tmp->next->content) > 1 
					|| get_arr(tmp->next->content) != NULL))
		{
			tmp_path_tokens = tmp->next->content;
			pattern_to_array(tmp_path_tokens);
			arr = get_arr(tmp_path_tokens);
			if (arr != NULL)
				replace_path_list(&tmp->next, arr);
			tmp = *path_list;
		}
		else
			tmp = tmp->next;
	}
}

int		path_exist(char *pathname)
{
	struct stat s;

	if (stat(pathname, &s) == -1 && (errno == ENOENT || errno == EACCES))
		return (0);	
	return (1);
}

t_list	*get_dir_list_tokens(t_list *path_list)
{
	t_list	*res;
	t_list	*tmp;
	char	*path;

	res = NULL;
	tmp = path_list;
	while (tmp != NULL)
	{
		path = ((t_token*)((t_list*)tmp->content)->content)->value;
		if (path_exist(path) == 1)
		{
			ft_lstadd_back(&res, 
				ft_lstnew(create_token(change_to_one(path, '/'), e_state_nsc)));
		}
		tmp = tmp->next;
	}
	return (res);
}

void	free_path_list(t_list *path_list)
{
	if (path_list != NULL && path_list->next != NULL)
		free_path_list(path_list->next);
	if (path_list != NULL)
	{
		free_path_tokens(path_list->content);
		free(path_list);
	}
}

t_list	*matched_dir_list_test(char *pattern)
{
	t_list	*path_list;
	t_list	*path_tokens;
	t_list	*dir_list;
	char *simplifyed_pattern;

	path_tokens = NULL;
	simplifyed_pattern = change_to_one(pattern, '*');
	path_tokens = split_path_tokens(simplifyed_pattern, path_tokens);
	join_same_type(path_tokens, (enum e_state)(e_path_path), 0);
	pattern_to_array(path_tokens);
	path_list = ft_lstnew(path_tokens);
	expand(&path_list);
	dir_list = get_dir_list_tokens(path_list);
	free_path_list(path_list);
	free(simplifyed_pattern);
	return (dir_list);
}

/*
** this will set_type of token with correspandante one
*/

t_list *set_type(t_list *tokens)
{
	t_list *tmp;

	tmp = tokens;
	while (tmp != NULL)
	{
		if (ft_strchr(((t_token*)tmp->content)->value, '*') != NULL)
			((t_token*)tmp->content)->type = (enum e_state)e_path_pattern;
		else
			((t_token*)tmp->content)->type = (enum e_state)e_path_path;
		tmp = tmp->next;
	}
	return (tokens);
}

void	push_token(t_list **tokens, char *start, char *end)
{
	char *t;

	t = (char*)malloc(sizeof(char) * (end - start + 1));
	ft_strlcpy(t, start, end - start + 1);
	ft_lstadd_back(tokens, ft_lstnew(create_token(t, 0)));

}

t_list *split_path_tokens(char *str, t_list *tokens)
{
	int i;
	char *start;
//	char *t;
	char *end;
	int change;

	start = str;
	change = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (change == 1)
		{
			change = 0;
			end = start + i;
			push_token(&tokens, start, end);
		//	t = (char*)malloc(sizeof(char) * (end - start + 1));
		//	ft_strlcpy(t, start, end - start + 1);
		//	ft_lstadd_back(&tokens, ft_lstnew(create_token(t, 0)));
			//str = end;
			start = end;
			i = 0;
		}
		if (start[i] == '/')
			change = 1;
		i++;
	}
	if (*start != '\0')
		ft_lstadd_back(&tokens, ft_lstnew(create_token(ft_strdup(start), 0)));
	return (set_type(tokens));
}

//int main(int argc, char **argv)
//{
//	//	char **dir_arr;
//	//	char **dir_arr_tmp;
//	//
//	//	dir_arr = get_dir_arr();
//	//	dir_arr_tmp = dir_arr;
//	//	if (argc == 2)
//	//	{
//	//		while (*dir_arr != NULL)
//	//		{
//	//			if (match(argv[1], *dir_arr, 0, 0))
//	//				printf("%s\n", *dir_arr);
//	//			dir_arr++;
//	//		}
//	//	}
//	//	else
//	//	{
//	//		printf("argum...!\n");
//	//	}
//	//	free_dir_arr(dir_arr_tmp);
//	char	*string;
//	int		m;
//	t_list	*res;
//	char	*pattern;
//	t_list	*path_tokens;
//	char	**p_arr;
//	int i;
//	DIR *d;
//
//	string = ".swsw";
//	m = match(".*", string, 0, 0);
//	string = "aaaabaaaabbbbaabbbaabbaababbabbaaaababaaabbbbbbaabbbabababbaaabaabaaaaaabbaabbbbaababbababaabbbaababbbba";
//	//printf("%s\n", change_to_one("*****b*aba***babaa*bbaba***a*aaba*b*aa**a*b**ba***a*a*"));
//	//printf("*b*aba*babaa*bbaba*a*aaba*b*aa*a*b*ba*a*a*");
//	//m = match(change_to_one("*****b*aba***babaa*bbaba***a*aaba*b*aa**a*b**ba***a*a*"), string, 0, 0);
//	m = match("*b*aba*babaa*bbaba*a*aaba*b*aa*a*b*ba*a*a*", string, 0, 0);
//	printf("%s\n", (m == 1) ? "match" : "doesn't match");
//	pattern= "/.*/.*/.*/.*/.*/.*";
//	//pattern = ".*";//"*/*ft/.*/*/*";
//	//p_arr = ft_split(pattern, '/');
//	//res = matched_dir_list(get_dir_arr_test(NULL, NULL, pattern), pattern);
//	//	path_tokens = split_path_tokens(pattern);
//	//	print_token(path_tokens);
//
//	//	join_same_type(path_tokens, (enum e_state)(e_path_path));
//	//	duplicate(path_tokens, 3);
//	//p_arr = get_dir_arr_test(".", ft_strchr(".*/", '/'), ".*/");
//	//path_exist("/../S+ Private Directory Data");
//	//printf("%s\n", strerror(errno));
//	//*(&argv) = NULL;
//	matched_dir_list_test(pattern);
//	//while(1);
//	//	append_slash(p_arr, (char*)1);
//	//	pattern_to_array(path_tokens);
//	//	while (path_tokens != NULL)
//	//	{
//	//		if (((t_token*)path_tokens->content)->type == e_path_array)
//	//		{
//	//			p_arr = (char**)((t_token*)path_tokens->content)->value;
//	//		}
//	//		else
//	//		{
//	//			printf("%s", ((t_token*)path_tokens->content)->value);
//	//			printf(" %d\n", ((t_token*)path_tokens->content)->type);
//	//		}
//	//		path_tokens = path_tokens->next;
//	//	}
//	//	i = 0;
//	//	while (p_arr[i] != NULL)
//	//	{
//	//		printf("%s\n", p_arr[i]);
//	//		i++;
//	//	}
//	//	while (path_tokens != NULL)
//	//	{
//	//		printf("%s", ((t_token*)path_tokens->content)->value);
//	//		printf(" %d\n", ((t_token*)path_tokens->content)->type);
//	//		path_tokens = path_tokens->next;
//	//	}
//	//-----------------------------------------------------------------
//	//	i = 0;
//	//	while (p_arr[i] != NULL)
//	//	{
//	//		printf("%s\n", p_arr[i]);
//	//		i++;
//	//	}
//	//	while (res != NULL)
//	//	{
//	//		printf("%s\n", ((t_token*)res->content)->value);
//	//		res = res->next;
//	//	}
//	//printf("%d\n", match("t*st", "t*stvkfmkmt", 0, 0));
//	return (0);
//}
