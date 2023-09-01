/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 15:36:55 by okraus            #+#    #+#             */
/*   Updated: 2023/09/01 13:29:09 by okraus           ###   ########.fr       */
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
[number_of_times_each_philosopher_must_eat]\n\
<number_of_philosophers>: 1-250\n<time_to_die>:            1-99999\n\
<time_to_eat>:            1-99999\n<time_to_sleep>:          1-99999\n\
[number_of_times_each_philosopher_must_eat]: 1-99999\n"
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


//	ft_philo.c
void	*ft_philo(void *data);

//other_functions.c
void	ft_free_table(t_table *table);
void	ft_destroy_mutexes(t_table *table);
t_table	*ft_inittable(char *argv[]);

void	ft_write_status(t_philo *philo, bool reaper_report, t_status status);
void	ft_write_outcome(t_table *table);

void	*ft_reaper(void *data);
int		ft_stop(t_table *table);

time_t	ft_get_time_in_ms(void);
void	ft_sim_start_delay(time_t start_time);


int		ft_atoi(char *str);
int		ft_check_input(int argc, char *argv[]);

#endif
