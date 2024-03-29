/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:15:14 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 15:38:20 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

static pthread_mutex_t	*ft_init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (NULL);
		i++;
	}
	return (forks);
}

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

static t_philo	**ft_init_philosophers(t_table *table)
{
	t_philo	**philos;
	int		i;

	philos = (t_philo **)malloc(sizeof(t_philo *) * (table->nb_philos + 1));
	if (!philos)
		return (NULL);
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (NULL);
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (NULL);
		philos[i]->table = table;
		ft_init_philosophers2(philos[i], i);
		philos[i]->times_ate = 0;
		philos[i]->last_meal = 0;
		ft_assign_forks(philos[i]);
		i++;
	}
	philos[i] = NULL;
	return (philos);
}

static int	ft_init_global_mutexes(t_table *table)
{
	table->fork_locks = ft_init_forks(table);
	if (!table->fork_locks)
		return (0);
	if (pthread_mutex_init(&table->sim_stop_lock, 0) != 0)
		return (1);
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
		return (1);
	if (pthread_mutex_init(&table->extra_lock, 0) != 0)
		return (1);
	return (0);
}

t_table	*ft_inittable(char *argv[])
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
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
