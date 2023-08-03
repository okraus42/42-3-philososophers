CC		= gcc
CFLAGS	= -Werror -Wall -Wextra -pthread -g
NAME	= philo_bonus

# Run 'make re MODE=pretty' for color formatting.
# Run 'make re MODE=debug' for debug formatting.
MODE	= none
ifeq ($(MODE), pretty)
	CFLAGS	+= -D DEBUG_FORMATTING=1
endif
ifeq ($(MODE), debug)
	CFLAGS	+= -D DEBUG_FORMATTING=1 -fsanitize=thread -g
endif

SRC_PATH = src/
OBJ_PATH = objects/

SRC		=	ft_philosophers_bonus.c \
			ft_parsing.c \
			ft_reaper.c \
			ft_time.c \
			ft_philo.c \
			ft_philo_utils.c \
			ft_init.c \
			ft_utils.c \
			ft_ipc.c \
			ft_output.c \
			ft_cleanup.c \
			ft_exit.c
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