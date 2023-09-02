/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:21:12 by okraus            #+#    #+#             */
/*   Updated: 2023/09/02 18:28:50 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

int	ft_error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		ft_free_table2(table);
	ft_error_msg(str, details, NULL, NULL);
	return (0);
}

void	*ft_error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		ft_free_table2(table);
	ft_error_msg(str, details, NULL, NULL);
	return (NULL);
}
