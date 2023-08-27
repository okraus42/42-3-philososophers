/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:43:40 by okraus            #+#    #+#             */
/*   Updated: 2023/08/27 14:11:43 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

static void	ft_eat_sleep_routine(t_philo *philo)
{
	ft_grab_fork(philo);
	ft_grab_fork(philo);
	sem_wait(philo->sem_meal);
	philo->last_meal = ft_get_time_in_ms();
	sem_post(philo->sem_meal);
	ft_write_status(philo, false, EATING);
	philo_sleep(philo->table->time_to_eat);
	sem_wait(philo->sem_meal);
	philo->times_ate += 1;
	sem_post(philo->sem_meal);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	philo->nb_forks_held -= 2;
	ft_write_status(philo, false, SLEEPING);
	philo_sleep(philo->table->time_to_sleep);
}

static void	ft_think_routine_even(t_philo *philo)
{
	time_t	time_to_think;

	sem_wait(philo->sem_meal);
	time_to_think = (philo->table->time_to_eat
			- philo->table->time_to_sleep);
	sem_post(philo->sem_meal);
	if (time_to_think < 0)
		time_to_think = 0;
	ft_write_status(philo, false, THINKING);
	philo_sleep(time_to_think);
}

static void	ft_think_routine_odd(t_philo *philo)
{
	time_t	time_to_think;

	sem_wait(philo->sem_meal);
	time_to_think = (philo->table->time_to_eat * 2
			- philo->table->time_to_sleep);
	sem_post(philo->sem_meal);
	if (time_to_think < 0)
		time_to_think = 0;
	ft_write_status(philo, false, THINKING);
	philo_sleep(time_to_think);
}

static void	ft_philosopher_routine(t_philo *philo)
{
	if (!(philo->id & 1))
		usleep(64 * philo->table->nb_philos);
	if (philo->table->nb_philos & 1)
	{
		while (1)
		{
			ft_eat_sleep_routine(philo);
			ft_think_routine_odd(philo);
		}
	}
	else
	{
		while (1)
		{
			ft_eat_sleep_routine(philo);
			ft_think_routine_even(philo);
		}
	}
}

void	ft_philosopher(t_table *table)
{
	t_philo	*philo;

	philo = table->this_philo;
	if (philo->table->nb_philos == 1)
		ft_lone_philo_routine(philo);
	ft_init_philo_ipc(table, philo);
	if (philo->table->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		ft_child_exit(table, ft_child_exit_PHILO_FULL);
	}
	if (philo->table->time_to_die == 0)
	{
		sem_post(philo->sem_philo_dead);
		ft_child_exit(table, ft_child_exit_PHILO_DEAD);
	}
	sem_wait(philo->sem_meal);
	philo->last_meal = philo->table->start_time;
	sem_post(philo->sem_meal);
	ft_sim_start_delay(philo->table->start_time);
	ft_philosopher_routine(philo);
}
