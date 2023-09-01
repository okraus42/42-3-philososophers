# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/04 09:44:51 by okraus            #+#    #+#              #
#    Updated: 2023/09/01 18:01:06 by okraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= cc
CFLAGS	= -Werror -Wall -Wextra -pthread -g
NAME	= philo_bonus

SRC_PATH = srcs/
OBJ_PATH = objects/

SRC		=	ft_philosophers_bonus.c \
			ft_other_functions.c \
			ft_philo.c

SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

INC		= -I ./includes/

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean
