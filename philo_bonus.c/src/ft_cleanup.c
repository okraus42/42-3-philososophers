/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 16:23:21 by mcombeau          #+#    #+#             */
/*   Updated: 2023/08/03 15:58:12 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

/* free_table:
*	Frees all of the memory allocated by the program.
*	Returns a NULL pointer if there is nothing to free,
*	or when all memory has been freed.
*/
void	*free_table(t_table *table)
{
	unsigned int	i;

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

/* sem_error_cleanup:
*	Closes and unlinks all semaphores. Used only during
*	initialization, in case a semaphore fails to be opened.
*	Returns 0 for failure.
*/
int	sem_error_cleanup(t_table *table)
{
	sem_close(table->sem_forks);
	sem_close(table->sem_write);
	sem_close(table->sem_philo_full);
	sem_close(table->sem_philo_dead);
	sem_close(table->sem_stop);
	unlink_global_sems();
	return (error_failure(STR_ERR_SEM, NULL, table));
}

/* table_cleanup:
*	Closes all semaphores and frees any allocated memory.
*	Also waits for the global grim reaper thread to terminate.
*	Returns the provided exit code.
*/
int	table_cleanup(t_table *table, int exit_code)
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
		unlink_global_sems();
		free_table(table);
	}
	return (exit_code);
}