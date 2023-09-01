/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_other_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 13:10:54 by okraus            #+#    #+#             */
/*   Updated: 2023/09/01 18:44:58 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_philosophers_bonus.h"

//todo - utils
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


//todo - utils
char	*ft_utoa(unsigned int nb, size_t len)
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


//todo - utils
void	ft_unlink_global_sems(void)
{
	sem_unlink(SEM_NAME_FORKS);
	sem_unlink(SEM_NAME_WRITE);
	sem_unlink(SEM_NAME_FULL);
	sem_unlink(SEM_NAME_DEAD);
	sem_unlink(SEM_NAME_STOP);
}

//todo - utils
bool	ft_start_grim_reaper_threads(t_table *table)
{
	if (pthread_create(&table->gluttony_reaper, NULL,
			&ft_global_gluttony_reaper, table) != 0)
		return (ft_error_failure(STR_ERR_THREAD, NULL, table));
	if (pthread_create(&table->famine_reaper, NULL,
			&ft_global_famine_reaper, table) != 0)
		return (ft_error_failure(STR_ERR_THREAD, NULL, table));
	return (true);
}

//todo - time
time_t	ft_get_time_in_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

//todo - time
void	philo_sleep(time_t sleep_time)
{
	time_t	wake_up;

	wake_up = ft_get_time_in_ms() + sleep_time;
	while (ft_get_time_in_ms() < wake_up)
	{
		usleep(100);
	}
}

//todo - time
void	ft_sim_start_delay(time_t start_time)
{
	while (ft_get_time_in_ms() < start_time)
	{
		usleep(100);
		continue ;
	}
}


//todo - reapers
int	ft_kill_all_philos(t_table *table, int exit_code)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		kill(table->pids[i], SIGKILL);
		i++;
	}
	return (exit_code);
}


//todo - reapers
void	*ft_global_gluttony_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count < 0 || table->time_to_die == 0
		|| table->nb_philos == 1)
		return (NULL);
	ft_sim_start_delay(table->start_time);
	while (table->philo_full_count < table->nb_philos)
	{
		if (ft_has_simulation_stopped(table) == true)
			return (NULL);
		sem_wait(table->sem_philo_full);
		if (ft_has_simulation_stopped(table) == false)
			table->philo_full_count += 1;
		else
			return (NULL);
	}
	sem_wait(table->sem_stop);
	table->stop_sim = true;
	ft_kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_philo_dead);
	sem_post(table->sem_stop);
	return (NULL);
}


//todo - reapers
void	*ft_global_famine_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->nb_philos == 1)
		return (NULL);
	ft_sim_start_delay(table->start_time);
	if (ft_has_simulation_stopped(table) == true)
		return (NULL);
	sem_wait(table->sem_philo_dead);
	if (ft_has_simulation_stopped(table) == true)
		return (NULL);
	sem_wait(table->sem_stop);
	table->stop_sim = true;
	ft_kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_philo_full);
	sem_post(table->sem_stop);
	return (NULL);
}


//todo - reapers
static bool	ft_end_condition_reached(t_table *table, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (ft_get_time_in_ms() - philo->last_meal >= table->time_to_die)
	{
		ft_write_status(philo, true, DIED);
		sem_post(table->this_philo->sem_philo_dead);
		sem_post(philo->sem_meal);
		return (true);
	}
	if (table->must_eat_count != -1 && philo->ate_enough == false
		&& philo->times_ate >= table->must_eat_count)
	{
		sem_post(philo->sem_philo_full);
		philo->ate_enough = true;
	}
	sem_post(philo->sem_meal);
	return (false);
}

//todo - reapers
void	*ft_personal_grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	sem_wait(table->this_philo->sem_philo_dead);
	sem_wait(table->this_philo->sem_philo_full);
	ft_sim_start_delay(table->start_time);
	while (!ft_end_condition_reached(table, table->this_philo))
	{
		usleep(100);
		continue ;
	}
	return (NULL);
}


//todo - output
void	ft_print_status(t_philo *philo, char *str)
{
	printf("\e[30;48:2:%i:%i:%im", philo->red, philo->green, philo->blue);
	printf("%6ld", ft_get_time_in_ms() - philo->table->start_time);
	printf("  %3d %-30s\e[0m\n", philo->id + 1, str);
}

//todo - output
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


//todo - ipc
static bool	ft_philo_open_global_semaphores(t_philo *philo)
{
	philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (false);
	philo->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_write == SEM_FAILED)
		return (false);
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		return (false);
	philo->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_dead == SEM_FAILED)
		return (false);
	return (true);
}

//todo - ipc
static bool	ft_philo_open_local_semaphores(t_philo *philo)
{
	philo->sem_meal = sem_open(philo->sem_meal_name, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_meal == SEM_FAILED)
		return (false);
	sem_unlink(philo->sem_meal_name);
	return (true);
}

//todo - ipc
void	ft_init_philo_ipc(t_table *table, t_philo *philo)
{
	if (table->nb_philos == 1)
		return ;
	sem_unlink(philo->sem_meal_name);
	if (!ft_philo_open_global_semaphores(philo))
		ft_child_exit(table, ft_child_exit_ERR_SEM);
	if (!ft_philo_open_local_semaphores(philo))
		ft_child_exit(table, ft_child_exit_ERR_SEM);
	if (pthread_create(&philo->ft_personal_grim_reaper, NULL,
			&ft_personal_grim_reaper, table) != 0)
		ft_child_exit(table, ft_child_exit_ERR_PTHREAD);
	return ;
}


//todo - exit (possibly not needed)
int	ft_error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		ft_free_table2(table);
	ft_error_msg(str, details, NULL, NULL);
	return (0);
}

//todo - exit 
void	*ft_error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		ft_free_table2(table);
	ft_error_msg(str, details, NULL, NULL);
	return (NULL);
}

//todo - exit 
void	ft_child_exit(t_table *table, int exit_code)
{
	sem_post(table->this_philo->sem_meal);
	pthread_join(table->this_philo->ft_personal_grim_reaper, NULL);
	if (exit_code == ft_child_exit_ERR_SEM)
		ft_error_msg(STR_ERR_SEM, NULL, NULL, NULL);
	if (exit_code == ft_child_exit_ERR_PTHREAD)
		ft_error_msg(STR_ERR_THREAD, NULL, NULL, NULL);
	sem_close(table->this_philo->sem_forks);
	sem_close(table->this_philo->sem_philo_full);
	sem_close(table->this_philo->sem_philo_dead);
	sem_close(table->this_philo->sem_write);
	sem_close(table->this_philo->sem_meal);
	ft_free_table2(table);
	exit(exit_code);
}

//done - utils
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

//done new
static int	ft_putstrerror(char *str)
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


//done - exit
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

//done - parsing
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

//done - parsing
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

//done - parsing
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
	return (true);
}


//todo - init
static char	*ft_set_local_sem_name(const char *str, int id)
{
	int	i;
	int	digit_count;
	char			*sem_name;
	char			*tmp;

	digit_count = 0;
	i = id;
	while (i)
	{
		digit_count++;
		i /= 10;
	}
	i = ft_strlen(str) + digit_count;
	sem_name = malloc (sizeof * sem_name * (i + 1));
	if (sem_name == NULL)
		return (NULL);
	sem_name[0] = '\0';
	sem_name = ft_strcat(sem_name, str);
	tmp = ft_utoa(id, digit_count);
	sem_name = ft_strcat(sem_name, tmp);
	free(tmp);

	return (sem_name);
}

//todo - init
static int	ft_set_philo_sem_names(t_philo *philo)
{
	philo->sem_meal_name = ft_set_local_sem_name(SEM_NAME_MEAL, philo->id + 1);
	if (philo->sem_meal_name == NULL)
		return (0);
	return (1);
}

//todo - init
static t_philo	**ft_init_philosophers(t_table *table)
{
	t_philo			**philos;
	int	i;

	philos = malloc(sizeof(t_philo) * (table->nb_philos + 1));
	if (!philos)
		return (ft_error_null(STR_ERR_MALLOC, "PHILOS\n", 0));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (ft_error_null(STR_ERR_MALLOC, "PHILOS[i]\n", 0));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->red = ((i / 4) + 12 * i) % 64 + 192;
		philos[i]->green = (32 + (i / 4) + 34 * i) % 64 + 192;
		philos[i]->blue = ((i / 4) + i * 16) % 64 + 192;
		if (!ft_set_philo_sem_names(philos[i]))
			return (ft_error_null(STR_ERR_MALLOC, "SEM NAMES\n", table));
		philos[i]->times_ate = 0;
		philos[i]->nb_forks_held = 0;
		philos[i]->ate_enough = 0;
		i++;
	}
	philos[i] = NULL;
	return (philos);
}

//todo - init
static bool	ft_init_global_semaphores(t_table *table)
{
	ft_unlink_global_sems();
	table->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_forks == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	table->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (table->sem_write == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	table->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_philo_full == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	table->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_philo_dead == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	table->sem_stop = sem_open(SEM_NAME_STOP, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (table->sem_stop == SEM_FAILED)
		return (ft_sem_error_cleanup(table));
	return (true);
}

//done - init
t_table	*ft_init_table(int argc, char *argv[])
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (ft_error_msg(STR_ERR_MALLOC, "TABLE\n", NULL, NULL), NULL);
	table->nb_philos = ft_mini_atoi(argv[1]);
	table->time_to_die = ft_mini_atoi(argv[2]);
	table->time_to_eat = ft_mini_atoi(argv[3]);
	table->time_to_sleep = ft_mini_atoi(argv[4]);
	table->must_eat_count = -1;
	table->philo_full_count = 0;
	table->stop_sim = 0;
	if (argc == 6)
		table->must_eat_count = ft_mini_atoi(argv[5]);
	if (!ft_init_global_semaphores(table))
		return (NULL);
	table->philos = ft_init_philosophers(table);
	if (!table->philos)
		return (NULL);
	table->pids = malloc(sizeof * table->pids * table->nb_philos);
	if (!table->pids)
		return (ft_error_msg(STR_ERR_MALLOC, "PIDS\n", NULL, NULL), NULL);
	return (table);
}

//todo - cleanup
int	ft_sem_error_cleanup(t_table *table)
{
	sem_close(table->sem_forks);
	sem_close(table->sem_write);
	sem_close(table->sem_philo_full);
	sem_close(table->sem_philo_dead);
	sem_close(table->sem_stop);
	ft_unlink_global_sems();
	return (ft_error_failure(STR_ERR_SEM, NULL, table));
}

//done - cleanup
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

//done - cleanup
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

//done - main/philosophers_bonus
int	ft_has_simulation_stopped(t_table *table)
{
	int	ret;

	sem_wait(table->sem_stop);
	ret = table->stop_sim;
	sem_post(table->sem_stop);
	return (ret);
}
