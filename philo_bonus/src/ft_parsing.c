/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:43:51 by okraus            #+#    #+#             */
/*   Updated: 2023/08/27 14:15:21 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

static bool	ft_contains_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	ft_integer_atoi(char *str)
{
	unsigned long long int	nb;
	unsigned int			i;

	i = 0;
	nb = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (nb > INT_MAX)
		return (-1);
	return ((int)nb);
}

bool	ft_is_valid_input(int ac, char **av)
{
	unsigned int	i;
	int				nb;

	i = 1;
	while (i < (unsigned int)ac)
	{
		if (!ft_contains_only_digits(av[i]))
			return (ft_msg(STR_ERR_INPUT_DIGIT, av[i], false));
		nb = ft_integer_atoi(av[i]);
		if (i == 1 && (nb <= 0 || nb > MAX_PHILOS))
			return (ft_msg(STR_ERR_INPUT_POFLOW, STR_MAX_PHILOS, false));
		if (i != 1 && nb == -1)
			return (ft_msg(STR_ERR_INPUT_DIGIT, av[i], false));
		i++;
	}
	return (true);
}
