/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 09:43:36 by okraus            #+#    #+#             */
/*   Updated: 2023/08/30 13:14:35 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"




int	main(int argc, char *argv[])
{
	t_table	*table;

	table = NULL;
	if (argc < 5 || argc > 6)
		return (ft_error_msg(STR_USAGE, NULL, NULL, NULL));
	if (!ft_is_valid_input(argc, argv))
		return (1);
	(void)table;
	return (0);
}
