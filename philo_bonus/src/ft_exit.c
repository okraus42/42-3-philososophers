/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:44:24 by okraus            #+#    #+#             */
/*   Updated: 2023/09/01 12:31:13 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

//done
int	ft_msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

//done
int	ft_error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		ft_free_table(table);
	return (ft_msg(str, details, 0));
}

//done
void	*ft_error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		ft_free_table(table);
	ft_msg(str, details, EXIT_FAILURE);
	return (NULL);
}

//done
void	ft_child_exit(t_table *table, int exit_code)
{
	sem_post(table->this_philo->sem_meal);
	pthread_join(table->this_philo->ft_personal_grim_reaper, NULL);
	if (exit_code == ft_child_exit_ERR_SEM)
		ft_msg(STR_ERR_SEM, NULL, 0);
	if (exit_code == ft_child_exit_ERR_PTHREAD)
		ft_msg(STR_ERR_THREAD, NULL, 0);
	sem_close(table->this_philo->sem_forks);
	sem_close(table->this_philo->sem_philo_full);
	sem_close(table->this_philo->sem_philo_dead);
	sem_close(table->this_philo->sem_write);
	sem_close(table->this_philo->sem_meal);
	ft_free_table(table);
	exit(exit_code);
}
