/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 11:09:32 by abel-mak          #+#    #+#             */
/*   Updated: 2021/01/17 12:12:15 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int match(char *pattern, char *string, int p, int s)
{
	int ret;

	while (p < ft_strlen(pattern))
	{
		//&& s < ft_strlen(string))
		//'*' match '.' if it's not the first char
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
			//printf("%s\n", dir->d_name);
		}
		closedir(d);
	}
	return (len);
}

void sort_dir_arr(char **dir_arr)
{
	int i;
	char *tmp;

	i = 1;
	while (dir_arr[i] != NULL)
	{
		if (dir_arr[i - 1][0] > dir_arr[i][0])
		{
			tmp = dir_arr[i - 1];
			dir_arr[i - 1] = dir_arr[i];
			dir_arr[i] = tmp;
			i = 0;
		}
		i++;
	}
}

char **get_dir_arr()
{
	DIR *d;
	struct dirent *dir;
	size_t i;
	char **dir_arr;

	dir_arr = (char**)malloc(sizeof(char*) * (dirlen(".") + 1));
	i = 0;
	d = opendir(".");
	while ((dir = readdir(d)) != NULL)
	{
		dir_arr[i] = ft_strdup(dir->d_name);
		i++;
	}
	closedir(d);
	dir_arr[i] = NULL;
	sort_dir_arr(dir_arr);
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

//int main(int argc, char **argv)
//{
////	char **dir_arr;
////	char **dir_arr_tmp;
////
////	dir_arr = get_dir_arr();
////	dir_arr_tmp = dir_arr;
////	if (argc == 2)
////	{
////		while (*dir_arr != NULL)
////		{
////			if (match(argv[1], *dir_arr, 0, 0))
////				printf("%s\n", *dir_arr);
////			dir_arr++;
////		}
////	}
////	else
////	{
////		printf("argum...!\n");
////	}
////	free_dir_arr(dir_arr_tmp);
//	char *string;
//	int m;
//	string = ".swsw";
//	m = match(".*", string, 0, 0);
//	string = "aaaabaaaabbbbaabbbaabbaababbabbaaaababaaabbbbbbaabbbabababbaaabaabaaaaaabbaabbbbaababbababaabbbaababbbba";
//	printf("%s\n", change_to_one("*****b*aba***babaa*bbaba***a*aaba*b*aa**a*b**ba***a*a*"));
//	printf("*b*aba*babaa*bbaba*a*aaba*b*aa*a*b*ba*a*a*");
//	//m = match(change_to_one("*****b*aba***babaa*bbaba***a*aaba*b*aa**a*b**ba***a*a*"), string, 0, 0);
//	m = match("*b*aba*babaa*bbaba*a*aaba*b*aa*a*b*ba*a*a*", string, 0, 0);
//	//printf("%s\n", (m == 1) ? "match" : "doesn't match");
//	//printf("%d\n", match("t*st", "t*stvkfmkmt", 0, 0));
//	return (0);
//}
