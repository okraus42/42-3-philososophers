/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:14:46 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 15:30:25 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

time_t	ft_get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_sim_start_delay(time_t start_time)
{
	while (ft_get_time_in_ms() < start_time)
	{
		usleep(400);
		continue ;
	}
}

void	ft_free_table(t_table *table)
{
	int	i;

	if (!table)
		return ;
	if (table->fork_locks != NULL)
		free(table->fork_locks);
	table->fork_locks = NULL;
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
				free(table->philos[i]);
			table->philos[i] = NULL;
			i++;
		}
		free(table->philos);
		table->philos = NULL;
	}
	free(table);
}

void	ft_destroy_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		pthread_mutex_destroy(&table->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_stop_lock);
	pthread_mutex_destroy(&table->extra_lock);
}

int	ft_error_msg(void)
{
	write(2, STR_USAGE, 323);
	return (1);
}
