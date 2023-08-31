/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:43:36 by okraus            #+#    #+#             */
/*   Updated: 2023/08/31 15:35:40 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"
//done

bool	ft_has_simulation_stopped(t_table *table)
{
	bool	ret;

	sem_wait(table->sem_stop);
	ret = table->stop_sim;
	sem_post(table->sem_stop);
	return (ret);
}
//done

static bool	ft_start_simulation(t_table *table)
{
	unsigned int	i;
	pid_t			pid;

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
	if (ft_start_grim_reaper_threads(table) == false)
		return (false);
	return (true);
}
//done

static int	ft_get_child_philo(t_table *table, pid_t *pid)
{
	int	philo_exit_code;
	int	exit_code;

	if (*pid && waitpid(*pid, &philo_exit_code, WNOHANG) != 0)
	{
		if (WIFEXITED(philo_exit_code))
		{
			exit_code = WEXITSTATUS(philo_exit_code);
			if (exit_code == ft_child_exit_PHILO_DEAD)
				return (ft_kill_all_philos(table, 1));
			if (exit_code == ft_child_exit_ERR_PTHREAD
				|| exit_code == ft_child_exit_ERR_SEM)
				return (ft_kill_all_philos(table, -1));
			if (exit_code == ft_child_exit_PHILO_FULL)
			{
				table->philo_full_count += 1;
				return (1);
			}
		}
	}
	return (0);
}
//done

static int	ft_stop_simulation(t_table	*table)
{
	unsigned int	i;
	int				exit_code;

	ft_sim_start_delay(table->start_time);
	while (ft_has_simulation_stopped(table) == false)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			exit_code = ft_get_child_philo(table, &table->pids[i]);
			if (exit_code == 1 || exit_code == -1)
			{
				sem_wait(table->sem_stop);
				table->stop_sim = true;
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
//done

int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (ft_msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!ft_is_valid_input(ac, av))
		return (EXIT_FAILURE);
	table = ft_init_table(ac, av);
	if (!table)
		return (EXIT_FAILURE);
	if (!ft_start_simulation(table))
		return (EXIT_FAILURE);
	if (ft_stop_simulation(table) == -1)
		return (ft_table_cleanup(table, EXIT_FAILURE));
	return (ft_table_cleanup(table, EXIT_SUCCESS));
}
