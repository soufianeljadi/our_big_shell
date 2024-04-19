# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/12 23:52:42 by sdiouane          #+#    #+#              #
#    Updated: 2024/04/19 15:43:15 by sdiouane         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc #-g -fsanitize=address 
CFLAGS = -Wall -Wextra -Werror 
RDFLAGS =  -L $(shell brew --prefix readline)/lib -lreadline
SRC = minishell.c \
		ft_itoa.c \
      parsing/ft_split.c \
	  parsing/lsts.c \
	  parsing/token.c \
	  parsing/parsing.c \
	  parsing/ft_strtock.c \
	  parsing/split_args.c \
	  parsing/split_by_pipe.c \
	  parsing/add_sep.c \
	  parsing/parse_pipe.c \
	  parsing/parse_redirection.c \
	  parsing/ft_strchr.c \
	  parsing/quotes.c \
	  parsing/ft_substr.c \
	  parsing/ft_strlen.c \
	  builtins/builtins.c \
	  builtins/export.c \
	  builtins/pwd.c \
	  builtins/cd.c \
	  builtins/unset.c \
	  builtins/echo.c \
      signals/signal.c \
	  execution/find_path.c \
	  execution/execution.c \
	  execution/redirections.c \
	  execution/helper.c \
	  gnl/get_next_line.c \
	  gnl/get_next_line_utils.c \

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -o $(NAME) $(RDFLAGS)

all: $(NAME)

%.o : %.c minishell.h
	@$(CC) $(CFLAGS) -I $(shell brew --prefix readline)/include -c $< -o $@
	
clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
