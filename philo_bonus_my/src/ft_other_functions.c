/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_other_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 13:10:54 by okraus            #+#    #+#             */
/*   Updated: 2023/08/31 15:34:22 by okraus           ###   ########.fr       */
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

t_table	*ft_init_table(int argc, char *argv[])
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (ft_error_msg(STR_ERR_MALLOC, NULL, NULL, NULL), NULL);
	table->nb_philos = ft_mini_atoi(argv[1]);
	table->time_to_die = ft_mini_atoi(argv[2]);
	table->time_to_eat = ft_mini_atoi(argv[3]);
	table->time_to_sleep = ft_mini_atoi(argv[4]);
	table->must_eat_count = -1;
	table->philo_full_count = 0;
	table->stop_sim = 0;
	if (argc == 6)
		table->must_eat_count = ft_integer_atoi(argv[5]);
	if (!ft_init_global_semaphores(table))
		return (NULL);
	table->philos = ft_init_philosophers(table);
	if (!table->philos)
		return (NULL);
	table->pids = malloc(sizeof * table->pids * table->nb_philos);
	if (!table->pids)
		return (ft_error_msg(STR_ERR_MALLOC, NULL, NULL, NULL), NULL);
	return (table);
}

void	*ft_free_table2(t_table *table)
{
	int	i;

	if (!table)
		return (NULL);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
			{
				if (table->philos[i]->sem_meal_name)
					free(table->philos[i]->sem_meal_name);
				free(table->philos[i]);
			}
			i++;
		}
		free(table->philos);
	}
	if (table->pids)
		free(table->pids);
	free(table);
	return (NULL);
}

int	ft_free_table(t_table *table, int exit_code)
{
	if (table != NULL)
	{
		pthread_join(table->famine_reaper, NULL);
		pthread_join(table->gluttony_reaper, NULL);
		sem_close(table->sem_forks);
		sem_close(table->sem_write);
		sem_close(table->sem_philo_full);
		sem_close(table->sem_philo_dead);
		sem_close(table->sem_stop);
		ft_unlink_global_sems();
		ft_free_table2(table);
	}
	return (exit_code);
}

int	ft_has_simulation_stopped(t_table *table)
{
	int	ret;

	sem_wait(table->sem_stop);
	ret = table->stop_sim;
	sem_post(table->sem_stop);
	return (ret);
}
