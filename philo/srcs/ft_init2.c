/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:37:59 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 15:38:33 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers.h"

void	ft_init_philosophers2(t_philo *philo, int i)
{
	philo->id = i;
	philo->red = ((i / 4) + 12 * i) % 64 + 192;
	philo->green = (32 + (i / 4) + 34 * i) % 64 + 192;
	philo->blue = ((i / 4) + i * 16) % 64 + 192;
}
