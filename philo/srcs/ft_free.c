/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 13:00:14 by okraus            #+#    #+#             */
/*   Updated: 2023/07/23 13:03:21 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

void	ft_free_table(t_table *table)
{
	int	i;

	if (!table)
		return ;
	if (table->fork_locks != NULL)
		free(table->fork_locks);
	table->fork_locks = NULL;
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
				free(table->philos[i]);
			table->philos[i] = NULL;
			i++;
		}
		free(table->philos);
		table->philos = NULL;
	}
	free(table);
}