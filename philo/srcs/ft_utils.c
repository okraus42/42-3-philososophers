/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 12:48:12 by okraus            #+#    #+#             */
/*   Updated: 2023/07/24 15:44:59 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

int	ft_atoi(char *str)
{
	long long int	n;
	int				i;

	i = 0;
	n = 0;
	if (str[i] < '0' || str[i] > '9')
		return (-1);
	while (str[i] && (str[i] >= '0' && str[i] <= '9' && i < 7)) //check condition
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] && i < 7)
		return (-2);
	if (i > 6) //check condition
		return (-3);
	if (n > 99999)
		return (-3);
	return ((int)n);
}

int	ft_check_input(int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (ft_atoi(argv[1]) < 1)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (ft_atoi(argv[2]) < 0)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (ft_atoi(argv[3]) < 0)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (ft_atoi(argv[4]) < 0)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	if (argv[5] && ft_atoi(argv[5]) < 0)
	{
		write(2, STR_USAGE, 135);
		return (1);
	}
	return (0);
}