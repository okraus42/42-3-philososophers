/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_other_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 13:10:54 by okraus            #+#    #+#             */
/*   Updated: 2023/08/30 13:33:48 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		++i;
	}
	return (i);
}

int	ft_putstrerror(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		write(2, &str[i], 1);
		++i;
	}
	return (i);
}

int	ft_error_msg(char *str, char *detail1, char *detail2, char *detail3)
{
	//write(2, str, ft_strlen(str) + 1);
	//write(2, "bbb\n", 4);
	ft_putstrerror(str);
	if (detail1)
		ft_putstrerror(detail1);
		//write(2, detail1, ft_strlen(detail1));
	//write(2, "aa\n", 3);
	if (detail2)
		ft_putstrerror(detail2);
		//write(2, detail2, ft_strlen(detail2));
	//write(2, "ccc\n", 4);
	if (detail3)
		ft_putstrerror(detail3);
		//write(2, detail3, ft_strlen(detail3));
	//printf ("s: %s \nd1: %s \nd2: %s \nd3: %s\n", str, detail1, detail2, detail3);
	return (1);
}

int	ft_contains_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		++i;
	}
	return (1);
}


// if not a digit already checked, no need to check for it here
int	ft_mini_atoi(char *str)
{
	int	nb;
	int	i;

	i = 0;
	nb = 0;
	while (str[i] && i < 8)
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] || nb > 99999)
		return (-1);
	return (nb);
}

int	ft_is_valid_input(int argc, char *argv[])
{
	int	i;
	int	nb;

	i = 1;
	while (i < argc)
	{
		if (!ft_contains_only_digits(argv[i]))
			return (ft_error_msg(STR_ERR_INPUT_DIGIT1, argv[i],	STR_ERR_INPUT_DIGIT2, NULL));
		nb = ft_mini_atoi(argv[i]);
		if (i == 1 && (nb <= 0 || nb > MAX_PHILOS))
			return (ft_error_msg(STR_ERR_INPUT_POFLOW1, STR_MAX_PHILOS,	STR_ERR_INPUT_POFLOW2, NULL));
		if (i != 1 && nb == -1)
			return (ft_error_msg(STR_ERR_INPUT_DIGIT1, argv[i],	STR_ERR_INPUT_DIGIT2, NULL));
		++i;
	}
	return (true);
}