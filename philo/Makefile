# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/22 15:33:07 by okraus            #+#    #+#              #
#    Updated: 2023/07/23 13:56:50 by okraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo
CC		= gcc
CFLAGS	= -Werror -Wall -Wextra -pthread

# Run 'make re MODE=pretty' for color formatting.
# Run 'make re MODE=debug' for debug formatting.
MODE	= none
ifeq ($(MODE), pretty)
	CFLAGS	+= -D DEBUG_FORMATTING=1
endif
ifeq ($(MODE), debug)
	CFLAGS	+= -D DEBUG_FORMATTING=1 -fsanitize=thread -g
endif

SRC_PATH = srcs/
OBJ_PATH = objs/

SRC		=	ft_philosophers.c \
			ft_free.c \
			ft_init.c \
			ft_philo.c \
			ft_print.c \
			ft_time.c \
			ft_utils.c
#			parsing.c \
#			grim_reaper.c \
#			time.c \
#			philosopher.c \
#			init.c \
#			output.c \
#			exit.c
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
