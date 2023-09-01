/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 15:39:22 by okraus            #+#    #+#             */
/*   Updated: 2023/09/01 12:19:59 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

//todo - philo-utils
void	ft_grab_fork(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_meal);
	if (philo->nb_forks_held <= 0)
		ft_write_status(philo, false, GOT_FORK_1);
	if (philo->nb_forks_held == 1)
		ft_write_status(philo, false, GOT_FORK_2);
	philo->nb_forks_held += 1;
	sem_post(philo->sem_meal);
}

//todo - philo-utils
void	ft_lone_philo_routine(t_philo *philo)
{
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		exit(ft_child_exit_ERR_SEM);
	sem_wait(philo->sem_philo_full);
	ft_sim_start_delay(philo->table->start_time);
	if (philo->table->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		exit(ft_child_exit_PHILO_FULL);
	}
	ft_print_status(philo, "has taken a fork");
	philo_sleep(philo->table->time_to_die);
	ft_print_status(philo, "died");
	ft_free_table2(philo->table);
	exit(ft_child_exit_PHILO_DEAD);
}

//todo - philo
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

//todo - philo
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

//todo - philo
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

//todo - philo
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

//todo - philo
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
