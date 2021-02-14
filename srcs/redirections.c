/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:04:12 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/14 18:34:18 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redirect_to_std_out(char *name_file)
{
    int fd;

    if ((fd = open(name_file, O_CREAT | O_RDWR | O_TRUNC, 0666)) < 0)
        ft_putendl_fd(strerror(errno), 2);
    close(fd);
}

char *get_input_fd(t_list *redir)
{
    char *file_name;

    file_name = ft_strdup("");
    while (redir)
    {
        if (((t_redir *)redir->content)->type == e_state_lt)
        {
            ft_free_arr((void **)&file_name);
            file_name = ft_strdup(((t_redir *)redir->content)->file);
        }
        redir = redir->next;
    }
    return (file_name);
}

char *get_output_fd(t_list *redir)
{
    char *file_name;

    file_name = ft_strdup("");
    while (redir)
    {
        if (((t_redir *)redir->content)->type == e_state_gt)
        {
            ft_free_arr((void **)&file_name);
            file_name = ft_strdup(((t_redir *)redir->content)->file);
            redirect_to_std_out(file_name);
        }
        redir = redir->next;
    }
    return (file_name);
}

void redir_is_in_cmd(t_exec *exec, t_cmd *cmd)
{
    t_list *tmp_red;
    char *infile;
    char *outfile;
    int fd_in;
    int fd_out;

    (void)exec;
    (void)cmd;
    tmp_red = cmd->redir_list;
    if (ft_strlen((infile = get_input_fd(tmp_red))) != 0)
    {
        if ((fd_in = open(infile, O_RDONLY)) == -1)
        {
            ft_free_arr((void **)&infile);
            ft_putendl_fd(strerror(errno), 2);
            exit_function(1);
        }
        ft_free_arr((void **)&infile);
    }
    else
        fd_in = dup(0);
    dup2(fd_in, 0);
    close(fd_in);
    if (ft_strlen((outfile = get_output_fd(tmp_red))) != 0)
    {
        if ((fd_out = open(outfile, O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
        {
            ft_free_arr((void **)&outfile);
            ft_putendl_fd(strerror(errno), 2);
            exit_function(1);
        }
        ft_free_arr((void **)&outfile);
    }
    else
        fd_out = dup(1);
    dup2(fd_out, 1);
    close(fd_out);
    exec->c_pid = fork();
    if (exec->c_pid == 0)
    {
        fill_args(cmd->word_list, exec);
        if (check_if_built_in(exec->args[0]))
            built_ins_execution(exec);
        else
        {
            if (get_cmd_path(exec))
                exec_cmd(exec);
        }
        exit_function(1);
    }
    else if (exec->c_pid == -1)
    {
        ft_putendl_fd(strerror(errno), 2);
        exit_function(1);
    }
    else
        waitpid(exec->c_pid, &exec->status, 0);
}

// while (tmp_red)
// {
//     if (((t_redir*)tmp_red->content)->type == e_state_gt)
//     {

//     // printf("************\n");
//         // fd = redirect_to_std_out(((t_redir*)tmp_red->content)->file);
//     }
//     tmp_red = tmp_red->next;
// }
// fill_args(cmd->word_list, exec);
// if (check_if_built_in(exec->args[0]))
//     built_ins_execution(exec);
// else
// {
//     if(get_cmd_path(exec))
//         exec_cmd(exec);
// }
// close(fd_out);

// check_for_failed()

// int redirect_to_std_in(char *name_file)
// {
//     int _fd;

//     if ((_fd = open(name_file, O_RDONLY, S_IRUSR)) < 0)
//         ft_putendl_fd(strerror(errno), 2);
//     return (_fd);
// }

// int redirect_to_std_in_append(char *name_file)
// {
//     int _fd;

//     if ((_fd = open(name_file, O_RDONLY, S_IRUSR)) < 0)
//         ft_putendl_fd(strerror(errno), 2);
//     return (_fd);
// }

// int redirect_to_std_out(char *name_file)
// {
//     int _fd;

//     if ((_fd = open(name_file, O_CREAT | O_RDWR | O_TRUNC, 0777)) < 0)
//         ft_putendl_fd(strerror(errno), 2);
//     return (_fd);
// }

// int get_ouput_fd(t_cmd *cmd)
// {
//     int fd;
//     t_list *_tmp_red;

//     _tmp_red = cmd->redir_list;
//     while (_tmp_red)
//     {
//         if (((t_redir *)_tmp_red->next)->type == e_state_gt)
//             fd = redirect_to_std_out(((t_redir *)_tmp_red->content)->file);
//     }
// }

// void redir_is_in_cmd(t_exec *exec, t_cmd *cmd)
// {
//     int fd;
//     // int tmp_fd;
//     (void)exec;
//     int size;
//     int i;
//     i = 0;

//     size = ft_lstsize(cmd->redir_list);
//     exec->pid_s = (int *)malloc(sizeof(int) * (size + 1));
//     while (cmd->redir_list)
//     {
//         exec->c_pid = fork(); //TODO: fork for each redirection
//         if (exec->c_pid == 0)
//         {
//             if (((t_redir *)cmd->redir_list->content)->type == e_state_gt)
//             {
//                 fd = redirect_to_std_out(((t_redir *)cmd->redir_list->content)->file);
//                 dup2(fd, 1);
//             }
//             else if (((t_redir *)cmd->redir_list->content)->type == e_state_lt)
//             {
//                 fd = redirect_to_std_in(((t_redir *)cmd->redir_list->content)->file);
//                 dup2(fd, 0);
//             }
//             close(fd);
//         }
//         else if (exec->c_pid == -1)
//             check_for_failed(strerror(errno));
//         else
//         {
//             waitpid(exec->c_pid, &exec->status, 0);
//             kill(exec->c_pid, SIGTERM);
//         }
//         if (check_if_built_in(exec->args[0]))
//             built_ins_execution(exec);
//         else
//         {
//             if (get_cmd_path(exec))
//                 exec_cmd(exec);
//         }
//         exec->pid_s[i++] = exec->c_pid;
//         cmd->redir_list = cmd->redir_list->next;
//     }
// }