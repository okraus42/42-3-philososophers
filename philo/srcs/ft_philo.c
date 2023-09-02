/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 12:37:33 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 15:23:32 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

static void	ft_eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	ft_write_status(philo, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	ft_write_status(philo, GOT_FORK_2);
	ft_write_status(philo, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = ft_get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	ft_philo_sleep(philo->table, philo->table->time_to_eat);
	if (!ft_stop(philo->table))
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	ft_write_status(philo, SLEEPING);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	ft_philo_sleep(philo->table, philo->table->time_to_sleep);
}

static void	ft_think_routine(t_philo *philo)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_to_think = (philo->table->time_to_eat
			- philo->table->time_to_sleep);
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	ft_write_status(philo, THINKING);
	ft_philo_sleep(philo->table, time_to_think);
}

static void	ft_think_routine_odd(t_philo *philo)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_to_think = (philo->table->time_to_eat * 2
			- philo->table->time_to_sleep);
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	ft_write_status(philo, THINKING);
	ft_philo_sleep(philo->table, time_to_think);
}

static void	ft_philo_routines(t_philo *philo)
{
	if (philo->table->nb_philos % 2)
	{
		while (!ft_stop(philo->table))
		{
			ft_eat_sleep_routine(philo);
			ft_think_routine_odd(philo);
		}
	}
	else
	{
		while (!ft_stop(philo->table))
		{
			ft_eat_sleep_routine(philo);
			ft_think_routine(philo);
		}
	}
}

void	*ft_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->table->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->table->extra_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->table->extra_lock);
	ft_sim_start_delay(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
	if (philo->id % 2 && philo->table->nb_philos > 1)
		usleep(64 * philo->table->nb_philos);
	if (philo->table->nb_philos == 1)
		return (ft_lone_philo_routine(philo));
	else
		ft_philo_routines(philo);
	return (NULL);
}
