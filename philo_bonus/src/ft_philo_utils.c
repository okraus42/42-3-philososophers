/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:43:47 by okraus            #+#    #+#             */
/*   Updated: 2023/08/27 14:11:53 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

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
	ft_free_table(philo->table);
	exit(ft_child_exit_PHILO_DEAD);
}
