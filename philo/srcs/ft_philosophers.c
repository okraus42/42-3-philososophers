/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 15:35:45 by okraus            #+#    #+#             */
/*   Updated: 2023/07/24 19:43:36 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

// void	ft_colourtest(void)
// {
// 	int	i;

// 	i = 0;
// 	while (i < 256)
// 	{
// 		printf("\e[30;48:2:%i:%i:%im%3i\e[0m", ((i / 4) + 12 * i) % 64 + 192, (32 + (i / 4) + 34 * i) % 64 + 192, ((i / 4) + i * 16) % 64 + 192, i);
// 		i++;
// 		if (!(i % 16))
// 			printf("\n");
// 	}
// 	printf("\n");
// 	i = 0;
// 	while (i < 256)
// 	{
// 		printf("r:%3i g:%3i b:%3i | ", ((i / 4) + 12 * i) % 64 + 192, (32 + (i / 4) + 34 * i) % 64 + 192, ((i / 4) + i * 16) % 64 + 192);
// 		i++;
// 		if (!(i % 4))
// 			printf("\n");
// 	}
// }

/* start_simulation:
*	Launches the simulation by creating a grim reaper thread as well as
*	one thread for each philosopher.
*	Returns true if the simulation was successfully started, false if there
*	was an error. 
*/
static bool	start_simulation(t_table *table)
{
	int	i;

	table->start_time = get_time_in_ms() + (table->nb_philos * 64);
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

/* stop_simulation:
*	Waits for all threads to be joined then destroys mutexes and frees
*	allocated memory.
*/
static void	stop_simulation(t_table	*table)
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
		write_outcome(table);
	destroy_mutexes(table);
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
	//ft_colourtest();
	if (start_simulation(table))
		return (3);
	stop_simulation(table);
	table = NULL;
	return (0);
}
