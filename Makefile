# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/24 09:35:20 by abel-mak          #+#    #+#              #
#    Updated: 2021/01/17 12:08:29 by abel-mak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= libft.a

all: $(NAME)
	gcc srcs/tokenizer.c srcs/wildcard.c srcs/check_envp_var.c -I ./headers -L ./libft -lft_bonus

$(NAME):
	@make bonus -sC ./libft
