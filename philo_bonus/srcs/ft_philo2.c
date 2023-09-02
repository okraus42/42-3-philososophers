/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 16:05:58 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:31:14 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

void	ft_grab_fork(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_meal);
	if (philo->nb_forks_held <= 0)
		ft_write_status(philo, 0, GOT_FORK_1);
	if (philo->nb_forks_held == 1)
		ft_write_status(philo, 0, GOT_FORK_2);
	philo->nb_forks_held += 1;
	sem_post(philo->sem_meal);
}

void	ft_lone_philo_routine(t_philo *philo)
{
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		exit(FT_CHILD_EXIT_ERR_SEM);
	sem_wait(philo->sem_philo_full);
	ft_sim_start_delay(philo->table->start_time);
	if (philo->table->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		exit(FT_CHILD_EXIT_PHILO_FULL);
	}
	ft_print_status(philo, "has taken a fork");
	philo_sleep(philo->table->time_to_die);
	ft_print_status(philo, "died");
	ft_free_table2(philo->table);
	exit(FT_CHILD_EXIT_PHILO_DEAD);
}
