/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:09:28 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:13:37 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

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

int	ft_is_valid_input(int argc, char *argv[])
{
	int	i;
	int	nb;

	i = 1;
	while (i < argc)
	{
		if (!ft_contains_only_digits(argv[i]))
			return (ft_error_msg(STR_USAGE, NULL, NULL, NULL), 0);
		nb = ft_mini_atoi(argv[i]);
		if (i == 1 && (nb <= 0 || nb > MAX_PHILOS))
			return (ft_error_msg(STR_USAGE, NULL, NULL, NULL), 0);
		if (i != 1 && nb == -1)
		{
			return (ft_error_msg(STR_USAGE, NULL, NULL, NULL), 0);
		}
		++i;
	}
	return (1);
}

void	ft_print_status(t_philo *philo, char *str)
{
	printf("\e[30;48:2:%i:%i:%im", philo->red, philo->green, philo->blue);
	printf("%6ld", ft_get_time_in_ms() - philo->table->start_time);
	printf("  %3d %-30s\e[0m\n", philo->id + 1, str);
}

void	ft_write_status(t_philo *philo, int reaper_report, t_status status)
{
	sem_wait(philo->sem_write);
	if (status == DIED)
		ft_print_status(philo, "died");
	else if (status == EATING)
		ft_print_status(philo, "is eating");
	else if (status == SLEEPING)
		ft_print_status(philo, "is sleeping");
	else if (status == THINKING)
		ft_print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		ft_print_status(philo, "has taken a fork");
	if (!reaper_report)
		sem_post(philo->sem_write);
}
