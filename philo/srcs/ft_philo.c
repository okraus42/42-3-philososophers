/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 13:44:59 by okraus            #+#    #+#             */
/*   Updated: 2023/08/03 12:38:00 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"


static void	ft_philo_sleep(t_table *table, time_t sleep_time)
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

static void	ft_eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	ft_write_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	ft_write_status(philo, false, GOT_FORK_2);
	ft_write_status(philo, false, EATING);
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
	ft_write_status(philo, false, SLEEPING);
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
	/*if (time_to_think > 600)
		time_to_think = 200;*/
	ft_write_status(philo, false, THINKING);
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
	/*if (time_to_think > 60000)
		time_to_think = 10000;*/
	ft_write_status(philo, false, THINKING);
	//write(1, "odd_is_thinking\n", 16);
	ft_philo_sleep(philo->table, time_to_think);
}

static void	*ft_lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	ft_write_status(philo, false, GOT_FORK_1);
	ft_philo_sleep(philo->table, philo->table->time_to_die);
	ft_write_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
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
	else if (philo->table->nb_philos % 2)
	{
		while (!ft_stop(philo->table))
		{
			//printf("philo %i\n", philo->id + 1);
			ft_eat_sleep_routine(philo);
			ft_think_routine_odd(philo);
		}
	}
	else
	{
		while (!ft_stop(philo->table))
		{
			//printf("philo %i\n", philo->id + 1);
			ft_eat_sleep_routine(philo);
			ft_think_routine(philo);
		}	
	}
	return (NULL);
}
