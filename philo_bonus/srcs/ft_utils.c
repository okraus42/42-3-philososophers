/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 16:01:57 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 16:03:19 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

time_t	ft_get_time_in_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	philo_sleep(time_t sleep_time)
{
	time_t	wake_up;

	wake_up = ft_get_time_in_ms() + sleep_time;
	while (ft_get_time_in_ms() < wake_up)
	{
		usleep(100);
	}
}

void	ft_sim_start_delay(time_t start_time)
{
	while (ft_get_time_in_ms() < start_time)
	{
		usleep(100);
		continue ;
	}
}

char	*ft_strcat(char	*dst, const char *src)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (dst[i])
		i++;
	j = 0;
	while (src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dst);
}

char	*ft_mini_itoa(int nb, size_t len)
{
	char	*ret;

	ret = malloc(sizeof * ret * (len + 1));
	if (!ret)
		return (NULL);
	ret[len] = '\0';
	len--;
	while (nb % 10)
	{
		ret[len--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (ret);
}
