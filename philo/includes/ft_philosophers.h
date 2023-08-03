/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 15:36:55 by okraus            #+#    #+#             */
/*   Updated: 2023/08/03 11:57:30 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

/*****************************************************************************\
|                                     Macros                                  |
\*****************************************************************************/

# define MAX_PHILOS		250
# define STR_MAX_PHILOS "250"

# ifndef DEBUG_FORMATTING
#  define DEBUG_FORMATTING 0
# endif

# define NC		"\e[0m"
# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define PURPLE	"\e[35m"
# define CYAN	"\e[36m"

# define STR_PROG_NAME			"philo:"
# define STR_USAGE				"philo: usage: ./philo \
<number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD			"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC			"%s error: Could not allocate memory.\n"
# define STR_ERR_MUTEX			"%s error: Could not create mutex.\n"

/*****************************************************************************\
|                                 Structures                                  |
\*****************************************************************************/

typedef struct s_philo	t_philo;

typedef struct s_table
{
	time_t			start_time;
	int				nb_philos;
	pthread_t		grim_reaper;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	bool			sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	extra_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	int					red;
	int					green;
	int					blue;
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_table				*table;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

/*****************************************************************************\
|                            Function Prototypes                              |
\*****************************************************************************/

//ft_free.c
void	ft_free_table(t_table *table);
void	ft_destroy_mutexes(t_table *table);

//	ft_init.c
t_table	*ft_inittable(char *argv[]);

//	ft_philo.c
void	*ft_philo(void *data);

//	ft_print.c
void	ft_write_status(t_philo *philo, bool reaper_report, t_status status);
void	ft_write_outcome(t_table *table);

//	ft_reaper.c
void	*ft_reaper(void *data);
int		ft_stop(t_table *table);

//	ft_time.c
time_t	ft_get_time_in_ms(void);
void	ft_sim_start_delay(time_t start_time);

//	ft_utils.c
int		ft_atoi(char *str);
int		ft_check_input(int argc, char *argv[]);

/*

//	parsing.c
bool			is_valid_input(int ac, char **av);
int				integer_atoi(char *str);

//	init.c
t_table			*init_table(int ac, char **av, int i);

//	routines.c
void			*philosopher(void *data);

//	time.c
time_t			get_time_in_ms(void);
void			philo_sleep(t_table *table, time_t sleep_time);
void			sim_start_delay(time_t start_time);

//	output.c
void			write_status(t_philo *philo, bool reaper, t_status status);
void			write_outcome(t_table *table);
void			*error_null(char *str, char *details, t_table *table);
int				msg(char *str, char *detail, int exit_no);

//	grim_reaper.c
void			*grim_reaper(void *data);
bool			has_simulation_stopped(t_table *table);

//	exit.c
int				error_failure(char *str, char *details, t_table *table);
void			*free_table(t_table *table);
void			destroy_mutexes(t_table *table);
*/

#endif
