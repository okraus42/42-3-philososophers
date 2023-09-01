/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_output.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:43:57 by okraus            #+#    #+#             */
/*   Updated: 2023/09/01 12:34:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

//done
void	ft_print_status(t_philo *philo, char *str)
{
	printf("\e[30;48:2:%i:%i:%im", philo->red, philo->green, philo->blue);
	printf("%6ld", ft_get_time_in_ms() - philo->table->start_time);
	printf("  %3d %-30s\e[0m\n", philo->id + 1, str);
}

//done
void	ft_write_status(t_philo *philo, bool reaper_report, t_status status)
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
