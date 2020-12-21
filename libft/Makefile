# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/24 16:03:10 by asaadi            #+#    #+#              #
#    Updated: 2020/11/10 11:49:11 by asaadi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

GG = gcc

FLAGS = -Wall -Wextra -Werror

AA = ar rc

RR = ranlib

SRC = ft_memset.c ft_bzero.c ft_memcpy.c ft_memccpy.c ft_memmove.c \
	  ft_memchr.c ft_memcmp.c ft_strlen.c ft_strlcpy.c ft_strlcat.c \
	  ft_strchr.c ft_strrchr.c ft_strnstr.c ft_strncmp.c ft_atoi.c \
	  ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	  ft_toupper.c ft_tolower.c ft_calloc.c ft_strdup.c ft_substr.c \
	  ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c \
	  ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

SRC_BONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
			ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
			ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

INC = libft.h

OBJ = $(SRC:.c=.o)

OBJ_BONUS = $(SRC_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(AA) $(NAME) $(OBJ)
	$(RR) $(NAME)

bonus: all $(OBJ_BONUS)
	$(AA) $(NAME) $(OBJ_BONUS)
	$(RR) $(NAME)

%.o: %.c $(INC)
	$(GG) $(FLAGS) -c $<

clean:
	@/bin/rm -rf $(OBJ) $(OBJ_BONUS)

fclean: clean
	@/bin/rm -rf $(NAME)

re: fclean all
