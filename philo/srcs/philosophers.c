/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 15:35:45 by okraus            #+#    #+#             */
/*   Updated: 2023/07/22 17:50:40 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_check_input(int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	(void)argv;
	return (0);
}

void	ft_colourtest(void)
{
	int	i;

	i = 0;
	while (i < 256)
	{
		printf("\e[30;48:2:%i:%i:%im%3i\e[0m", ((i / 4) + 12 * i) % 64 + 192, (32 + (i / 4) + 34 * i) % 64 + 192, ((i / 4) + i * 16) % 64 + 192, i);
		i++;
		if (!(i % 16))
			printf("\n");
	}
	printf("\n");
	i = 0;
	while (i < 256)
	{
		printf("r:%3i g:%3i b:%3i | ", ((i / 4) + 12 * i) % 64 + 192, (32 + (i / 4) + 34 * i) % 64 + 192, ((i / 4) + i * 16) % 64 + 192);
		i++;
		if (!(i % 4))
			printf("\n");
	}
}

int	main(int argc, char *argv[])
{
	t_table	*table;

	if (ft_check_input(argc, argv))
		return (1);
	(void)argv;
	table = NULL;
	(void)table;
	ft_colourtest();
	return (0);
}
