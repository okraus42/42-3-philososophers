/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:44:24 by okraus            #+#    #+#             */
/*   Updated: 2023/08/04 09:44:25 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

/* msg:
*	Writes a message to the console. Returns the provided exit number.
*	Used for error management.
*/
int	msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

/* error_failure:
*	Frees any allocated memory, prints an error message and
*	returns 0 to indicate failure.
*	Used for error management during initialization.
*/
int	error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	return (msg(str, details, 0));
}

/* error_null:
*	Frees any allocated memory, prints an error message and returns a NULL pointer.
*	Used for error management during initialization.
*/
void	*error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}

/* child_exit:
*	Exits a child philosopher process with the appropriate exit code.
*	Prints an error message if the child encountered an error.
*/
void	child_exit(t_table *table, int exit_code)
{
	sem_post(table->this_philo->sem_meal);
	pthread_join(table->this_philo->personal_grim_reaper, NULL);
	if (exit_code == CHILD_EXIT_ERR_SEM)
		msg(STR_ERR_SEM, NULL, 0);
	if (exit_code == CHILD_EXIT_ERR_PTHREAD)
		msg(STR_ERR_THREAD, NULL, 0);
	sem_close(table->this_philo->sem_forks);
	sem_close(table->this_philo->sem_philo_full);
	sem_close(table->this_philo->sem_philo_dead);
	sem_close(table->this_philo->sem_write);
	sem_close(table->this_philo->sem_meal);
	free_table(table);
	exit(exit_code);
}
