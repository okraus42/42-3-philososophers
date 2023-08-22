/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:43:13 by okraus            #+#    #+#             */
/*   Updated: 2023/08/22 11:58:52 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

/* get_time_in_ms:
*	Gets the current time in miliseconds since the Epoch (1970-01-01 00:00:00).
*	Returns the time value.
*/
time_t	ft_get_time_in_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* philo_sleep:
*	Pauses the philosopher process for a certain amount of time in miliseconds.
*/
void	philo_sleep(time_t sleep_time)
{
	time_t	wake_up;

	wake_up = ft_get_time_in_ms() + sleep_time;
	while (ft_get_time_in_ms() < wake_up)
	{
		usleep(100);
	}
}

/* sim_start_delay:
*	Waits for a small delay at the beginning of each process execution
*	so that all processes start at the same time with the same start time
*	reference. This ensures all processes and the grim reaper threads are
*	synchronized.
*/
void	sim_start_delay(time_t start_time)
{
	while (ft_get_time_in_ms() < start_time)
		continue ;
}