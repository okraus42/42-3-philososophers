/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:02:37 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:31:14 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

static int	ft_philo_open_global_semaphores(t_philo *philo)
{
	philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (0);
	philo->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_write == SEM_FAILED)
		return (0);
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		return (0);
	philo->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_dead == SEM_FAILED)
		return (0);
	return (1);
}

static int	ft_philo_open_local_semaphores(t_philo *philo)
{
	philo->sem_meal = sem_open(philo->sem_meal_name, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_meal == SEM_FAILED)
		return (0);
	sem_unlink(philo->sem_meal_name);
	return (1);
}

void	ft_init_philo_semaphores(t_table *table, t_philo *philo)
{
	if (table->nb_philos == 1)
		return ;
	sem_unlink(philo->sem_meal_name);
	if (!ft_philo_open_global_semaphores(philo))
		ft_child_exit(table, FT_CHILD_EXIT_ERR_SEM);
	if (!ft_philo_open_local_semaphores(philo))
		ft_child_exit(table, FT_CHILD_EXIT_ERR_SEM);
	if (pthread_create(&philo->ft_personal_grim_reaper, NULL,
			&ft_personal_grim_reaper, table) != 0)
		ft_child_exit(table, FT_CHILD_EXIT_ERR_PTHREAD);
	return ;
}

void	ft_init_philosophers2(t_philo *philo, int i)
{
	philo->id = i;
	philo->red = ((i / 4) + 12 * i) % 64 + 192;
	philo->green = (32 + (i / 4) + 34 * i) % 64 + 192;
	philo->blue = ((i / 4) + i * 16) % 64 + 192;
}
