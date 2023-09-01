/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 15:35:45 by okraus            #+#    #+#             */
/*   Updated: 2023/09/01 12:42:00 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

//todo - main
static bool	ft_start_simulation(t_table *table)
{
	int	i;

	table->start_time = ft_get_time_in_ms() + (table->nb_philos * 64);
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
				&ft_philo, table->philos[i]) != 0)
			return (1); //message
		i++;
	}
	if (table->nb_philos > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL,
				&ft_reaper, table) != 0)
			return (1); //message
	}
	return (0);
}

//todo - main
static void	ft_stop_simulation(t_table	*table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->nb_philos > 1)
		pthread_join(table->grim_reaper, NULL);
	if (DEBUG_FORMATTING == true && table->must_eat_count != -1)
		ft_write_outcome(table);
	ft_destroy_mutexes(table);
	ft_free_table(table);
}

//todo - main
int	main(int argc, char *argv[])
{
	t_table	*table;

	if (ft_check_input(argc, argv))
		return (1);
	table = ft_inittable(argv);
	if (!table)
		return (2);
	//ft_colourtest();
	if (ft_start_simulation(table))
		return (3);
	ft_stop_simulation(table);
	table = NULL;
	return (0);
}
