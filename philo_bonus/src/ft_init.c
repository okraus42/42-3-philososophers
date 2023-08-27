/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:44:19 by okraus            #+#    #+#             */
/*   Updated: 2023/08/27 14:20:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

static char	*ft_set_local_sem_name(const char *str, unsigned int id)
{
	unsigned int	i;
	unsigned int	digit_count;
	char			*sem_name;
	char			*tmp;

	digit_count = 0;
	i = id;
	while (i)
	{
		digit_count++;
		i /= 10;
	}
	i = ft_strlen(str) + digit_count;
	sem_name = malloc (sizeof * sem_name * (i + 1));
	if (sem_name == NULL)
		return (NULL);
	sem_name[0] = '\0';
	sem_name = ft_strcat(sem_name, str);
	tmp = ft_utoa(id, digit_count);
	sem_name = ft_strcat(sem_name, tmp);
	free(tmp);
	return (sem_name);
}

static bool	ft_set_philo_sem_names(t_philo *philo)
{
	philo->sem_meal_name = ft_set_local_sem_name(SEM_NAME_MEAL, philo->id + 1);
	if (philo->sem_meal_name == NULL)
		return (false);
	return (true);
}

static t_philo	**ft_init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * (table->nb_philos + 1));
	if (!philos)
		return (ft_error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (ft_error_null(STR_ERR_MALLOC, NULL, 0));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->red = ((i / 4) + 12 * i) % 64 + 192;
		philos[i]->green = (32 + (i / 4) + 34 * i) % 64 + 192;
		philos[i]->blue = ((i / 4) + i * 16) % 64 + 192;
		if (!ft_set_philo_sem_names(philos[i]))
			return (ft_error_null(STR_ERR_MALLOC, NULL, table));
		philos[i]->times_ate = 0;
		philos[i]->nb_forks_held = 0;
		philos[i]->ate_enough = false;
		i++;
	}
	philos[i] = NULL;
	return (philos);
}

static bool	ft_init_global_semaphores(t_table *table)
{
	ft_unlink_global_sems();
	table->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_forks == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	table->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (table->sem_write == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	table->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_philo_full == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	table->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_philo_dead == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	table->sem_stop = sem_open(SEM_NAME_STOP, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (table->sem_stop == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	return (true);
}

t_table	*ft_init_table(int ac, char **av)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (ft_error_null(STR_ERR_MALLOC, NULL, 0));
	table->nb_philos = ft_integer_atoi(av[1]);
	table->time_to_die = ft_integer_atoi(av[2]);
	table->time_to_eat = ft_integer_atoi(av[3]);
	table->time_to_sleep = ft_integer_atoi(av[4]);
	table->must_eat_count = -1;
	table->philo_full_count = 0;
	table->stop_sim = false;
	if (ac == 6)
		table->must_eat_count = ft_integer_atoi(av[5]);
	if (!ft_init_global_semaphores(table))
		return (NULL);
	table->philos = ft_init_philosophers(table);
	if (!table->philos)
		return (NULL);
	table->pids = malloc(sizeof * table->pids * table->nb_philos);
	if (!table->pids)
		return (ft_error_null(STR_ERR_MALLOC, NULL, 0));
	return (table);
}
