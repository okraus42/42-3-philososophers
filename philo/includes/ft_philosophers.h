/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 15:36:55 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:25:43 by okraus           ###   ########.fr       */
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

/*****************************************************************************\
|                                     Macros                                  |
\*****************************************************************************/

# define MAX_PHILOS		250

# define STR_PROG_NAME			"philo:"
# define STR_USAGE				"philo: usage: ./philo \
<number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n\
<number_of_philosophers>: 1-250\n<time_to_die>:            0-99999\n\
<time_to_eat>:            0-99999\n<time_to_sleep>:          0-99999\n\
[number_of_times_each_philosopher_must_eat]: 0-99999\n"

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
	int				sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	extra_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				times_ate;
	int				fork[2];
	int				red;
	int				green;
	int				blue;
	pthread_mutex_t	meal_time_lock;
	time_t			last_meal;
	t_table			*table;
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

//	ft_philo2.c
void	*ft_lone_philo_routine(t_philo *philo);
void	ft_philo_sleep(t_table *table, time_t sleep_time);

//	ft_init.c
t_table	*ft_inittable(char *argv[]);

//	ft_init2.c
void	ft_init_philosophers2(t_philo *philo, int i);

//	ft_utils.c
void	ft_write_status(t_philo *philo, t_status status);
int		ft_stop(t_table *table);
int		ft_atoi(char *str);
int		ft_check_input(int argc, char *argv[]);

//	ft_reaper.c
void	*ft_reaper(void *data);

//	ft_utils2.c
time_t	ft_get_time_in_ms(void);
void	ft_sim_start_delay(time_t start_time);
void	ft_free_table(t_table *table);
void	ft_destroy_mutexes(t_table *table);
int		ft_error_msg(void);

#endif
