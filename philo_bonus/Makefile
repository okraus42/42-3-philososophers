# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 18:45:53 by okraus            #+#    #+#              #
#    Updated: 2023/09/02 19:09:22 by okraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


ifndef ECHO
HIT_TOTAL	!=	${MAKE} ${MAKECMDGOALS} --dry-run ECHO="HIT_MARK" | grep -c "HIT_MARK"
HIT_COUNT1	=	$(eval HIT_N != expr ${HIT_N} + 1)${HIT_N}
HIT_COUNT2	=	$(eval HIT_M != expr ${HIT_M} + 1)${HIT_M}
HIT_COUNT3	=	$(eval HIT_O != expr ${HIT_O} + $(BAR_LENGTH))${HIT_O}
PERC		=	$(shell expr $(HIT_COUNT3) / $(HIT_TOTAL) | bc)
ECHO		=	echo "[`expr ${HIT_COUNT1} '*' 100 / ${HIT_TOTAL}`%]"
endif

NAME		=	philo_bonus
INCS		=	-I ./includes/
CC 			=	cc
CFLAGS		=	-Wall -Wextra -Werror -pthread -g
OBJ_PATH 	=	objects/
SLEEP		=	#sleep 0.5

# Source directories

SRC_PATH 	=	srcs/

# SOURCES

SRC			=	ft_philosophers_bonus.c \
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

SRCS		= $(addprefix $(SRC_PATH), $(SRC))

# Formating

NRM_FORMAT	=	\033[0m

GREEN		=	\033[1;32m
YELLOW		=	\033[1;93m
RED			=	\033[1;31m
REVERSE		=	\033[7m
RETURN		=	\033[1F\r\033[2K

# Printing

BAR_LENGTH	:=	50
PRINT1		=	printf "[$(HIT_COUNT2)/$(HIT_TOTAL)]" ;
PRINT2		=	for num in `seq 1 $(BAR_LENGTH)` ; do \
					if [ $$num -le $(PERC) ] ; then \
						printf "$(REVERSE) $(NRM_FORMAT)" ; \
					else \
						printf " " ; \
        			fi \
    			done

# Objects

OBJ			= $(SRC:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))

# Dependencies

DEP			= $(SRC:.c=.d)
DEPS		= $(addprefix $(OBJ_PATH), $(DEP))

# RULES

all:			announce $(NAME)
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)
				@echo "$(GREEN)Amazing <<$(REVERSE)Philosopher bonus$(NRM_FORMAT)$(GREEN)>> compiled!$(NRM_FORMAT)"
				
# MESSAGES 

announce: 
				@echo "$(RED)Magic in progress...\n$(NRM_FORMAT)"

# COMPILATION

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
				@mkdir -p $(OBJ_PATH)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling Philosophers bonus: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ $(INCS)
				@$(ECHO)

$(NAME): 		$(OBJS)
				@$(CC) $(CFLAGS) $(OBJS) -o $@
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)
				@echo "$(RETURN)$(RETURN)$(RETURN)$(GREEN)Philosophers bonus compiled!$(NRM_FORMAT)"

# add dependencies for headers and stuff somehow

-include $(DEPS)

# clean library

clean:
				@rm -rf $(OBJ_PATH)
				@echo "$(RED)Objects removed!$(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)

# Remove the program after running clean

fclean:			clean
				@rm $(NAME)
				@echo "$(RETURN)$(RED)Philosophers bonus deleted!$(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)

# Remove stuff and make it all again

re: 			fclean all
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)

# Phony stuff for rules

.PHONY: all announce clean fclean re