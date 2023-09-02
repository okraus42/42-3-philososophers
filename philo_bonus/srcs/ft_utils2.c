/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 16:03:27 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:28:18 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

void	ft_unlink_global_sems(void)
{
	sem_unlink(SEM_NAME_FORKS);
	sem_unlink(SEM_NAME_WRITE);
	sem_unlink(SEM_NAME_FULL);
	sem_unlink(SEM_NAME_DEAD);
	sem_unlink(SEM_NAME_STOP);
}

int	ft_has_simulation_stopped(t_table *table)
{
	int	ret;

	sem_wait(table->sem_stop);
	ret = table->stop_sim;
	sem_post(table->sem_stop);
	return (ret);
}

int	ft_strlen(const char *str)
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

int	ft_error_msg(char *str, char *detail1, char *detail2, char *detail3)
{
	ft_putstrerror(str);
	if (detail1)
		ft_putstrerror(detail1);
	if (detail2)
		ft_putstrerror(detail2);
	if (detail3)
		ft_putstrerror(detail3);
	return (1);
}
