/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:43:36 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:31:14 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

static int	ft_start_simulation(t_table *table)
{
	int		i;
	pid_t	pid;

	table->start_time = ft_get_time_in_ms() + ((table->nb_philos * 2) * 10);
	i = -1;
	while (++i < table->nb_philos)
	{
		pid = fork();
		if (pid == -1)
			return (ft_error_failure(STR_ERR_FORK, NULL, table));
		else if (pid > 0)
			table->pids[i] = pid;
		else if (pid == 0)
		{
			table->this_philo = table->philos[i];
			ft_philosopher(table);
		}
	}
	if (ft_start_grim_reaper_threads(table) == 0)
		return (0);
	return (1);
}

static int	ft_get_child_philo(t_table *table, pid_t *pid)
{
	int	philo_exit_code;
	int	exit_code;

	if (*pid && waitpid(*pid, &philo_exit_code, WNOHANG) != 0)
	{
		if (WIFEXITED(philo_exit_code))
		{
			exit_code = WEXITSTATUS(philo_exit_code);
			if (exit_code == FT_CHILD_EXIT_PHILO_DEAD)
				return (ft_kill_all_philos(table, 1));
			if (exit_code == FT_CHILD_EXIT_ERR_PTHREAD
				|| exit_code == FT_CHILD_EXIT_ERR_SEM)
				return (ft_kill_all_philos(table, -1));
			if (exit_code == FT_CHILD_EXIT_PHILO_FULL)
			{
				table->philo_full_count += 1;
				return (1);
			}
		}
	}
	return (0);
}

static int	ft_stop_simulation(t_table	*table)
{
	int	i;
	int	exit_code;

	ft_sim_start_delay(table->start_time);
	while (ft_has_simulation_stopped(table) == 0)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			exit_code = ft_get_child_philo(table, &table->pids[i]);
			if (exit_code == 1 || exit_code == -1)
			{
				sem_wait(table->sem_stop);
				table->stop_sim = 1;
				sem_post(table->sem_philo_full);
				sem_post(table->sem_philo_dead);
				sem_post(table->sem_stop);
				return (exit_code);
			}
			i++;
		}
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_table	*table;

	table = NULL;
	if (argc < 5 || argc > 6)
		return (ft_error_msg(STR_USAGE, NULL, NULL, NULL));
	if (!ft_is_valid_input(argc, argv))
		return (1);
	table = ft_init_table(argc, argv);
	if (!table)
		return (1);
	if (!ft_start_simulation(table))
		return (1);
	if (ft_stop_simulation(table) == -1)
		return (ft_free_table(table, EXIT_FAILURE));
	return (ft_free_table(table, EXIT_SUCCESS));
	return (0);
}
