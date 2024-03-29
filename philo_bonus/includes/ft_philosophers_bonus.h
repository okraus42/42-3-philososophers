/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers_bonus.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:44:34 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:31:14 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_BONUS_H
# define FT_PHILOSOPHERS_BONUS_H

# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <pthread.h>

/******************************************************************************
*                                     Macros                                  *
******************************************************************************/

# define MAX_PHILOS	250
# define STR_MAX_PHILOS "250"

# define STR_USAGE				"philo: usage: ./philo_bonus \
<number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n\
<number_of_philosophers>: 1-250\n<time_to_die>:            0-99999\n\
<time_to_eat>:            0-99999\n<time_to_sleep>:          0-99999\n\
[number_of_times_each_philosopher_must_eat]: 0-99999\n"
# define STR_ERR_THREAD	"philo_bonus: error: Could not create thread.\n"
# define STR_ERR_MALLOC	"philo_bonus: error: Could not allocate memory.\n"
# define STR_ERR_SEM	"philo_bonus: error: Could not create semaphore.\n"
# define STR_ERR_FORK	"philo_bonus: error: Could not fork child.\n"

# define SEM_NAME_FORKS "/philo_global_forks"
# define SEM_NAME_WRITE "/philo_global_write"
# define SEM_NAME_FULL	"/philo_global_full"
# define SEM_NAME_DEAD	"/philo_global_dead"
# define SEM_NAME_STOP	"/philo_global_stop"
# define SEM_NAME_MEAL	"/philo_local_meal_"

# define FT_CHILD_EXIT_ERR_PTHREAD	40
# define FT_CHILD_EXIT_ERR_SEM		41
# define FT_CHILD_EXIT_PHILO_FULL	42
# define FT_CHILD_EXIT_PHILO_DEAD	43

/******************************************************************************
*                                 Structures                                  *
******************************************************************************/

typedef struct s_philo	t_philo;

typedef struct s_table
{
	time_t		start_time;
	int			nb_philos;
	time_t		time_to_die;
	time_t		time_to_eat;
	time_t		time_to_sleep;
	int			must_eat_count;
	sem_t		*sem_forks;
	sem_t		*sem_write;
	sem_t		*sem_philo_full;
	int			philo_full_count;
	sem_t		*sem_philo_dead;
	sem_t		*sem_stop;
	int			stop_sim;
	t_philo		**philos;
	t_philo		*this_philo;
	pid_t		*pids;
	pthread_t	gluttony_reaper;
	pthread_t	famine_reaper;
}	t_table;

typedef struct s_philo
{
	pthread_t	ft_personal_grim_reaper;
	sem_t		*sem_forks;
	sem_t		*sem_write;
	sem_t		*sem_philo_full;
	sem_t		*sem_philo_dead;
	sem_t		*sem_meal;
	char		*sem_meal_name;
	int			nb_forks_held;
	int			id;
	int			times_ate;
	int			red;
	int			green;
	int			blue;
	int			ate_enough;
	time_t		last_meal;
	t_table		*table;
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

/******************************************************************************
*                           Function Prototypes                               *
******************************************************************************/

//	main.c

//	ft_utils.c
time_t	ft_get_time_in_ms(void);
void	philo_sleep(time_t sleep_time);
void	ft_sim_start_delay(time_t start_time);
char	*ft_strcat(char	*dst, const char *src);
char	*ft_mini_itoa(int nb, size_t len);

//	ft_utils2.c
void	ft_unlink_global_sems(void);
int		ft_has_simulation_stopped(t_table *table);
int		ft_strlen(const char *str);
int		ft_mini_atoi(char *str);
int		ft_error_msg(char *str, char *detail1, char *detail2, char *detail3);

//	ft_utils3.c
int		ft_putstrerror(char *str);
int		ft_is_valid_input(int argc, char *argv[]);
int		ft_contains_only_digits(char *str);
void	ft_print_status(t_philo *philo, char *str);
void	ft_write_status(t_philo *philo, int reaper_report, t_status status);

//	ft_philo.c
void	ft_philosopher(t_table *table);

//	ft_philo2.c
void	ft_lone_philo_routine(t_philo *philo);
void	ft_grab_fork(t_philo *philo);

//	ft_init.c
t_table	*ft_init_table(int argc, char *argv[]);

//	ft_init2.c
void	ft_init_philo_semaphores(t_table *table, t_philo *philo);
void	ft_init_philosophers2(t_philo *philo, int i);

//	ft_reapers.c
int		ft_start_grim_reaper_threads(t_table *table);
void	*ft_global_gluttony_reaper(void *data);
void	*ft_global_famine_reaper(void *data);
void	*ft_personal_grim_reaper(void *data);

//	ft_exit.c
int		ft_kill_all_philos(t_table *table, int exit_code);
int		ft_free_table(t_table *table, int exit_code);
void	*ft_free_table2(t_table *table);
int		ft_sem_error_cleanup(t_table *table);
void	ft_child_exit(t_table *table, int exit_code);

//	ft_exit2.c
int		ft_error_failure(char *str, char *details, t_table *table);
void	*ft_error_null(char *str, char *details, t_table *table);

#endif
