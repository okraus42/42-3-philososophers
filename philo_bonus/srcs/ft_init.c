/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:02:42 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:28:01 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

static char	*ft_set_local_sem_name(const char *str, int id)
{
	int		i;
	int		digit_count;
	char	*sem_name;
	char	*tmp;

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
	tmp = ft_mini_itoa(id, digit_count);
	sem_name = ft_strcat(sem_name, tmp);
	free(tmp);
	return (sem_name);
}

static int	ft_set_philo_sem_names(t_philo *philo)
{
	philo->sem_meal_name = ft_set_local_sem_name(SEM_NAME_MEAL, philo->id + 1);
	if (philo->sem_meal_name == NULL)
		return (0);
	return (1);
}

static t_philo	**ft_init_philosophers(t_table *table)
{
	t_philo		**philos;
	int			i;

	philos = malloc(sizeof(t_philo) * (table->nb_philos + 1));
	if (!philos)
		return (ft_error_null(STR_ERR_MALLOC, "PHILOS\n", 0));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (ft_error_null(STR_ERR_MALLOC, "PHILOS[i]\n", 0));
		philos[i]->table = table;
		ft_init_philosophers2(philos[i], i);
		if (!ft_set_philo_sem_names(philos[i]))
			return (ft_error_null(STR_ERR_MALLOC, "SEM NAMES\n", table));
		philos[i]->times_ate = 0;
		philos[i]->nb_forks_held = 0;
		philos[i]->ate_enough = 0;
		i++;
	}
	philos[i] = NULL;
	return (philos);
}

static int	ft_init_global_semaphores(t_table *table)
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
	return (1);
}

t_table	*ft_init_table(int argc, char *argv[])
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (ft_error_msg(STR_ERR_MALLOC, "TABLE\n", NULL, NULL), NULL);
	table->nb_philos = ft_mini_atoi(argv[1]);
	table->time_to_die = ft_mini_atoi(argv[2]);
	table->time_to_eat = ft_mini_atoi(argv[3]);
	table->time_to_sleep = ft_mini_atoi(argv[4]);
	table->must_eat_count = -1;
	table->philo_full_count = 0;
	table->stop_sim = 0;
	if (argc == 6)
		table->must_eat_count = ft_mini_atoi(argv[5]);
	if (!ft_init_global_semaphores(table))
		return (NULL);
	table->philos = ft_init_philosophers(table);
	if (!table->philos)
		return (NULL);
	table->pids = malloc(sizeof * table->pids * table->nb_philos);
	if (!table->pids)
		return (ft_error_msg(STR_ERR_MALLOC, "PIDS\n", NULL, NULL), NULL);
	return (table);
}
