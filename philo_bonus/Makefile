# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/04 09:44:51 by okraus            #+#    #+#              #
#    Updated: 2023/09/02 18:24:14 by okraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= cc
CFLAGS	= -Werror -Wall -Wextra -pthread -g
NAME	= philo_bonus

SRC_PATH = srcs/
OBJ_PATH = objects/

SRC		=	ft_philosophers_bonus.c \
			ft_philo.c \
			ft_philo2.c \
			ft_init.c \
			ft_init2.c \
			ft_reapers.c \
			ft_utils.c \
			ft_utils2.c \
			ft_utils3.c \
			ft_exit.c \
			ft_exit2.c

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
