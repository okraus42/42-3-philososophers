/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reaper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 15:02:16 by okraus            #+#    #+#             */
/*   Updated: 2023/08/03 12:31:21 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

static void	ft_set_sim_stop_flag(t_table *table, bool state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
		table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

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

