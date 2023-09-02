/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:21:14 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:31:14 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

int	ft_kill_all_philos(t_table *table, int exit_code)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		kill(table->pids[i], SIGKILL);
		i++;
	}
	return (exit_code);
}

int	ft_sem_error_cleanup(t_table *table)
{
	sem_close(table->sem_forks);
	sem_close(table->sem_write);
	sem_close(table->sem_philo_full);
	sem_close(table->sem_philo_dead);
	sem_close(table->sem_stop);
	ft_unlink_global_sems();
	return (ft_error_failure(STR_ERR_SEM, NULL, table));
}

void	*ft_free_table2(t_table *table)
{
	int	i;

	if (!table)
		return (NULL);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
			{
				if (table->philos[i]->sem_meal_name)
					free(table->philos[i]->sem_meal_name);
				free(table->philos[i]);
			}
			i++;
		}
		free(table->philos);
	}
	if (table->pids)
		free(table->pids);
	free(table);
	return (NULL);
}

int	ft_free_table(t_table *table, int exit_code)
{
	if (table != NULL)
	{
		pthread_join(table->famine_reaper, NULL);
		pthread_join(table->gluttony_reaper, NULL);
		sem_close(table->sem_forks);
		sem_close(table->sem_write);
		sem_close(table->sem_philo_full);
		sem_close(table->sem_philo_dead);
		sem_close(table->sem_stop);
		ft_unlink_global_sems();
		ft_free_table2(table);
	}
	return (exit_code);
}

void	ft_child_exit(t_table *table, int exit_code)
{
	sem_post(table->this_philo->sem_meal);
	pthread_join(table->this_philo->ft_personal_grim_reaper, NULL);
	if (exit_code == FT_CHILD_EXIT_ERR_SEM)
		ft_error_msg(STR_ERR_SEM, NULL, NULL, NULL);
	if (exit_code == FT_CHILD_EXIT_ERR_PTHREAD)
		ft_error_msg(STR_ERR_THREAD, NULL, NULL, NULL);
	sem_close(table->this_philo->sem_forks);
	sem_close(table->this_philo->sem_philo_full);
	sem_close(table->this_philo->sem_philo_dead);
	sem_close(table->this_philo->sem_write);
	sem_close(table->this_philo->sem_meal);
	ft_free_table2(table);
	exit(exit_code);
}
