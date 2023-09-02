/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reapers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:15:27 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:29:00 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

int	ft_start_grim_reaper_threads(t_table *table)
{
	if (pthread_create(&table->gluttony_reaper, NULL,
			&ft_global_gluttony_reaper, table) != 0)
		return (ft_error_failure(STR_ERR_THREAD, NULL, table));
	if (pthread_create(&table->famine_reaper, NULL,
			&ft_global_famine_reaper, table) != 0)
		return (ft_error_failure(STR_ERR_THREAD, NULL, table));
	return (1);
}

void	*ft_global_gluttony_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count < 0 || table->time_to_die == 0
		|| table->nb_philos == 1)
		return (NULL);
	ft_sim_start_delay(table->start_time);
	while (table->philo_full_count < table->nb_philos)
	{
		if (ft_has_simulation_stopped(table) == 1)
			return (NULL);
		sem_wait(table->sem_philo_full);
		if (ft_has_simulation_stopped(table) == 0)
			table->philo_full_count += 1;
		else
			return (NULL);
	}
	sem_wait(table->sem_stop);
	table->stop_sim = 1;
	ft_kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_philo_dead);
	sem_post(table->sem_stop);
	return (NULL);
}

void	*ft_global_famine_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->nb_philos == 1)
		return (NULL);
	ft_sim_start_delay(table->start_time);
	if (ft_has_simulation_stopped(table) == 1)
		return (NULL);
	sem_wait(table->sem_philo_dead);
	if (ft_has_simulation_stopped(table) == 1)
		return (NULL);
	sem_wait(table->sem_stop);
	table->stop_sim = 1;
	ft_kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_philo_full);
	sem_post(table->sem_stop);
	return (NULL);
}

static int	ft_end_condition_reached(t_table *table, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (ft_get_time_in_ms() - philo->last_meal >= table->time_to_die)
	{
		ft_write_status(philo, 1, DIED);
		sem_post(table->this_philo->sem_philo_dead);
		sem_post(philo->sem_meal);
		return (1);
	}
	if (table->must_eat_count != -1 && philo->ate_enough == 0
		&& philo->times_ate >= table->must_eat_count)
	{
		sem_post(philo->sem_philo_full);
		philo->ate_enough = 1;
	}
	sem_post(philo->sem_meal);
	return (0);
}

void	*ft_personal_grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	sem_wait(table->this_philo->sem_philo_dead);
	sem_wait(table->this_philo->sem_philo_full);
	ft_sim_start_delay(table->start_time);
	while (!ft_end_condition_reached(table, table->this_philo))
	{
		usleep(100);
		continue ;
	}
	return (NULL);
}
