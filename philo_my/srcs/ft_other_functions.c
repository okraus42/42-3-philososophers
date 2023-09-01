/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_other_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 12:37:29 by okraus            #+#    #+#             */
/*   Updated: 2023/09/01 12:45:44 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"


//todo - init
static pthread_mutex_t	*ft_init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
		return (NULL); //mesage
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (NULL); //message
		i++;
	}
	return (forks);
}


//todo - init
static void	ft_assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->nb_philos;
		philo->fork[1] = philo->id;
	}
}


//todo - init
static t_philo	**ft_init_philosophers(t_table *table)
{
	t_philo	**philos;
	int		i;

	philos = (t_philo **)malloc(sizeof(t_philo *) * (table->nb_philos + 1));
	if (!philos)
		return (NULL); //message
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (NULL); //message
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (NULL); //message
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->red = ((i / 4) + 12 * i) % 64 + 192;
		philos[i]->green = (32 + (i / 4) + 34 * i) % 64 + 192;
		philos[i]->blue = ((i / 4) + i * 16) % 64 + 192;
		philos[i]->times_ate = 0;
		philos[i]->last_meal = 0;
		ft_assign_forks(philos[i]);
		i++;
	}
	philos[i] = NULL;
	return (philos);
}

//todo - init
static int	ft_init_global_mutexes(t_table *table)
{
	table->fork_locks = ft_init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&table->sim_stop_lock, 0) != 0)
		return (1); //message
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
		return (1); //messsage
	if (pthread_mutex_init(&table->extra_lock, 0) != 0)
		return (1); //messsage
	return (0);
}

//todo - init
t_table	*ft_inittable(char *argv[])
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL); //add message
	table->nb_philos = ft_atoi(argv[1]);
	table->time_to_die = (time_t)ft_atoi(argv[2]);
	table->time_to_eat = (time_t)ft_atoi(argv[3]);
	table->time_to_sleep = (time_t)ft_atoi(argv[4]);
	table->must_eat_count = -1;
	if (argv[5])
		table->must_eat_count = ft_atoi(argv[5]);
	table->philos = ft_init_philosophers(table);
	if (!table->philos)
		return (NULL);
	if (ft_init_global_mutexes(table))
		return (NULL);
	table->sim_stop = 0;
	return (table);
}

//todo - print
static void	ft_print_status_debug(t_philo *philo, char *color,
								char *str, t_status status)
{
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			ft_get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str, philo->fork[0]);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			ft_get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str, philo->fork[1]);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n",
			ft_get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str);
}

//todo - print
static void	ft_write_status_debug(t_philo *philo, t_status status)
{
	if (status == DIED)
		ft_print_status_debug(philo, RED, "died", status);
	else if (status == EATING)
		ft_print_status_debug(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		ft_print_status_debug(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		ft_print_status_debug(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		ft_print_status_debug(philo, PURPLE, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		ft_print_status_debug(philo, PURPLE, "has taken a fork", status);
}

//todo - print
static void	ft_print_status(t_philo *philo, char *str)
{
	// printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
	// 	philo->id + 1, str);
	printf("\e[30;48:2:%i:%i:%im", philo->red, philo->green, philo->blue);
	printf("%6ld", ft_get_time_in_ms() - philo->table->start_time);
	printf("  %3d %-30s\e[0m\n", philo->id + 1, str);
}

//todo - print

static void	ft_print_status(t_philo *philo, char *str)
{
	// printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
	// 	philo->id + 1, str);
	printf("\e[30;48:2:%i:%i:%im", philo->red, philo->green, philo->blue);
	printf("%6ld", ft_get_time_in_ms() - philo->table->start_time);
	printf("  %3d %-30s\e[0m\n", philo->id + 1, str);
}


//todo - print
void	ft_write_status(t_philo *philo, bool reaper_report, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	(void)reaper_report;
	if (ft_stop(philo->table) == true && reaper_report == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (DEBUG_FORMATTING == true)
	{
		ft_write_status_debug(philo, status);
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		ft_print_status(philo, "died");
	else if (status == EATING)
		ft_print_status(philo, "is eating");
	else if (status == SLEEPING)
		ft_print_status(philo, "is sleeping");
	else if (status == THINKING)
		ft_print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		ft_print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}

//todo - print
void	ft_write_outcome(t_table *table)
{
	int				i;
	unsigned int	full_count;

	full_count = 0;
	i = 0;
	while (i < table->nb_philos)
	{
		if (table->philos[i]->times_ate >= (unsigned int)table->must_eat_count)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&table->write_lock);
	printf("%d/%d philosophers had at least %d meals.\n",
		full_count, table->nb_philos, table->must_eat_count);
	pthread_mutex_unlock(&table->write_lock);
	return ;
}

//todo - reaper
static void	ft_set_sim_stop_flag(t_table *table, bool state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
		table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

//todo - reaper
int	ft_stop(t_table *table)
{
	int	r;

	r = 0;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->sim_stop == true)
		r = 1;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (r);
}

//todo - reaper
static bool	ft_kill_philo(t_philo *philo)
{
	time_t	time;

	time = ft_get_time_in_ms();
	if ((time - philo->last_meal) >= philo->table->time_to_die)
	{
		ft_set_sim_stop_flag(philo->table, true);
		ft_write_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

//todo - reaper
static bool	ft_end_condition_reached(t_table *table)
{
	int		i;
	bool	all_ate_enough;

	all_ate_enough = true;
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_lock);
		if (ft_kill_philo(table->philos[i]))
			return (true);
		if (table->must_eat_count != -1)
			if (table->philos[i]->times_ate
				< (unsigned int)table->must_eat_count)
				all_ate_enough = false;
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		i++;
	}
	if (table->must_eat_count != -1 && all_ate_enough == true)
	{
		ft_set_sim_stop_flag(table, true);
		return (true);
	}
	return (false);
}

//todo - reaper
static int ft_init_check(t_table *table)
{
	int		i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_lock);
		if (!table->philos[i]->last_meal)
		{
			ft_set_sim_stop_flag(table, true);
			write(2, "Simulation initiation fail.\n",29);
			return (1);
		}
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		i++;
	}
	return (0);
}


//todo - reaper
void	*ft_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	ft_set_sim_stop_flag(table, false);
	ft_sim_start_delay(table->start_time);
	if (ft_init_check(table))
		return (NULL);
	while (true)
	{
		if (ft_end_condition_reached(table) == true)
			return (NULL);
		//usleep(1000);
		usleep(100);
	}
	return (NULL);
}


//todo - time
time_t	ft_get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

//todo - time

void	ft_sim_start_delay(time_t start_time)
{
	while (ft_get_time_in_ms() < start_time)
	{
		usleep(400);
		continue ;
	}
}

//to do - utils
int	ft_atoi(char *str)
{
	long long int	n;
	int				i;

	i = 0;
	n = 0;
	if (str[i] < '0' || str[i] > '9')
		return (-1);
	while (str[i] && (str[i] >= '0' && str[i] <= '9' && i < 7)) //check condition
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] && i < 7)
		return (-2);
	if (i > 6) //check condition
		return (-3);
	if (n > 99999)
		return (-3);
	return ((int)n);
}


//to do - utils
int	ft_check_input(int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (ft_atoi(argv[1]) < 1)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (ft_atoi(argv[2]) < 0)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (ft_atoi(argv[3]) < 0)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (ft_atoi(argv[4]) < 0)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (argv[5] && ft_atoi(argv[5]) < 0)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	return (0);
}