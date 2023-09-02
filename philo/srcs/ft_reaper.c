/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reaper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:14:45 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 15:31:24 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

static void	ft_set_sim_stop_flag(t_table *table, int state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
	table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

static int	ft_kill_philo(t_philo *philo)
{
	time_t	time;

	time = ft_get_time_in_ms();
	if ((time - philo->last_meal) >= philo->table->time_to_die)
	{
		ft_set_sim_stop_flag(philo->table, 1);
		ft_write_status(philo, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (1);
	}
	return (0);
}

static int	ft_end_condition_reached(t_table *table)
{
	int	i;
	int	all_ate_enough;

	all_ate_enough = 1;
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_lock);
		if (ft_kill_philo(table->philos[i]))
			return (1);
		if (table->must_eat_count != -1)
			if (table->philos[i]->times_ate
				< (int)table->must_eat_count)
				all_ate_enough = 0;
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		i++;
	}
	if (table->must_eat_count != -1 && all_ate_enough == 1)
	{
		ft_set_sim_stop_flag(table, 1);
		return (1);
	}
	return (0);
}

static int	ft_init_check(t_table *table)
{
	int		i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_lock);
		if (!table->philos[i]->last_meal)
		{
			ft_set_sim_stop_flag(table, 1);
			write(2, "Simulation initiation fail.\n", 29);
			return (1);
		}
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		i++;
	}
	return (0);
}

void	*ft_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	ft_set_sim_stop_flag(table, 0);
	ft_sim_start_delay(table->start_time);
	if (ft_init_check(table))
		return (NULL);
	while (1)
	{
		if (ft_end_condition_reached(table) == 1)
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
