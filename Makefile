# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 13:12:14 by lpaysant          #+#    #+#              #
#    Updated: 2025/08/28 17:06:20 by lpaysant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.Phony : clean fclean all re

CFLAGS = -Wall -Wextra -Werror -g3 -I./inc
NAME = philo
CC = cc
OBJDIR = ./obj
SRCDIR = ./src

SRC = \
	$(SRCDIR)/main.c \
	$(SRCDIR)/philo_utils.c

OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))

all : $(OBJDIR) $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJDIR)/%.o : $(SRCDIR)/%.c Makefile ./inc/philo.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean :
	rm -rf *.o $(OBJDIR)

fclean : clean
	rm -f $(NAME)

re : fclean all
