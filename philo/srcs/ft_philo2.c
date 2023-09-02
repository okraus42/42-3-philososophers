/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:10:06 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 15:11:05 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

void	ft_philo_sleep(t_table *table, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = ft_get_time_in_ms() + sleep_time;
	while (ft_get_time_in_ms() < wake_up)
	{
		(void)table;
		if (ft_stop(table))
			break ;
		usleep(100);
	}
}

void	*ft_lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	ft_write_status(philo, GOT_FORK_1);
	ft_philo_sleep(philo->table, philo->table->time_to_die);
	ft_write_status(philo, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}
