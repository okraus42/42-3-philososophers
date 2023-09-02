/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 15:35:45 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 19:24:27 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

static int	ft_start_simulation(t_table *table)
{
	int	i;

	table->start_time = ft_get_time_in_ms() + (table->nb_philos * 4);
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
				&ft_philo, table->philos[i]) != 0)
			return (1);
		i++;
	}
	if (table->nb_philos > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL,
				&ft_reaper, table) != 0)
			return (1);
	}
	return (0);
}

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
	ft_destroy_mutexes(table);
	ft_free_table(table);
}

int	main(int argc, char *argv[])
{
	t_table	*table;

	if (ft_check_input(argc, argv))
		return (1);
	table = ft_inittable(argv);
	if (!table)
		return (2);
	if (ft_start_simulation(table))
		return (3);
	ft_stop_simulation(table);
	table = NULL;
	return (0);
}
