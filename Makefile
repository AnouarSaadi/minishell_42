# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/07 12:43:35 by asaadi            #+#    #+#              #
#    Updated: 2021/02/28 14:42:46 by asaadi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

FLAGS = -Wall -Wextra -Werror

SRC_PATH = srcs

SRC_NAME = 	main.c\
			plx_tokenizer.c\
			plx_wildcard.c\
			ex_execution_commands.c\
			ex_built_in_pwd.c\
			ex_built_in_cd.c\
			ex_built_in_echo.c\
			ex_built_in_exit.c\
			ex_built_in_env.c\
			ex_built_in_export.c\
			ex_built_in_export_2.c\
			ex_built_in_unset.c\
			ex_get_envp_var.c\
			ex_get_the_binary_path.c\
			ex_get_the_binary_path_2.c\
			ex_no_leak_memory.c\
			ex_redirections.c\
			ex_pipe_execution.c\
			ex_execve_execution.c\
			ex_signals.c\

HDR_PATH = headers/
HDR_INC = -I ./headers
HDR_NAME = minishell.h libft.h

OBJ_PATH= objs
OBJ_NAME = $(SRC_NAME:.c=.o)

LIBFT_PATH = ./libft

OBJ= $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))
SRC= $(addprefix $(SRC_PATH)/,$(SRC_NAME))
HDR= $(addprefix $(HDR_PATH)/,$(HDR_NAME))

all: $(NAME) 

$(NAME): $(OBJ)
	@make -sC $(LIBFT_PATH)
	@cp libft/libft.a .
	@gcc $(FLAGS) libft.a $(OBJ) -o $(NAME)
	@echo "\033[0;33m"
	@echo "			COMPILATION DONE!\n"
	@echo "\033[0m"
	@echo "						Created by:"
	@echo "\033[1;30m"
	@echo "						  abdel-mak && asaadi\n"
	@echo "\033[0m"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(HDR)
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@gcc $(FLAGS) $(HDR_INC)  -o $@ -c $<

clean:
	@rm -rf $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@make clean -sC $(LIBFT_PATH)
	@echo "minishell : Removing Objs"

fclean: clean
	@rm -rf $(NAME)
	@rm -rf libft.a
	@make fclean -sC $(LIBFT_PATH)
	@echo "minishell : Removing minishell"

re: fclean all
