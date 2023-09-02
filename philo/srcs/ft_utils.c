/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:14:42 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 15:29:25 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

static void	ft_print_status(t_philo *philo, char *str)
{
	printf("\e[30;48:2:%i:%i:%im", philo->red, philo->green, philo->blue);
	printf("%6ld", ft_get_time_in_ms() - philo->table->start_time);
	printf("  %3d %-30s\e[0m\n", philo->id + 1, str);
}

void	ft_write_status(t_philo *philo, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (ft_stop(philo->table) == 1 && status != DIED)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
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
	pthread_mutex_unlock(&philo->table->write_lock);
}

int	ft_stop(t_table *table)
{
	int	r;

	r = 0;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->sim_stop == 1)
		r = 1;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (r);
}

int	ft_atoi(char *str)
{
	int	n;
	int	i;

	i = 0;
	n = 0;
	if (str[i] < '0' || str[i] > '9')
		return (-1);
	while (str[i] && (str[i] >= '0' && str[i] <= '9' && i < 7))
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] && i < 7)
		return (-2);
	if (i > 6)
		return (-3);
	if (n > 99999)
		return (-3);
	return (n);
}

int	ft_check_input(int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
		return (ft_error_msg());
	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > MAX_PHILOS)
		return (ft_error_msg());
	if (ft_atoi(argv[2]) < 0)
		return (ft_error_msg());
	if (ft_atoi(argv[3]) < 0)
		return (ft_error_msg());
	if (ft_atoi(argv[4]) < 0)
		return (ft_error_msg());
	if (argv[5] && ft_atoi(argv[5]) < 0)
		return (ft_error_msg());
	return (0);
}
