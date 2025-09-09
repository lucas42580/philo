/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:37:00 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/09 18:11:41 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	print_lock(t_phidata *philo, char *str)
{
	long			time_ms;
	struct timeval	time;

	pthread_mutex_lock(&philo->data->print_mutex);
	gettimeofday(&time, NULL);
	time_ms = get_time_ms(time) - philo->data->start_time;
	if (death_check(philo) == 1)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (-1);
	}
	printf("%ld %d %s", time_ms, philo->id, str);
	pthread_mutex_unlock(&philo->data->print_mutex);
	return (0);
}

int	ft_wait(t_phidata *philo, long time)
{
	struct timeval	start;
	struct timeval	end;

	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	while (get_time_ms(end) - get_time_ms(start) < time)
	{
		gettimeofday(&end, NULL);
		if (death_check(philo) == 1)
			return (-1);
		usleep(10);
	}
	return (0);
}

int	fork_taking(t_phidata *philo, pthread_mutex_t *mutex, int *fork)
{
	pthread_mutex_lock(mutex);
	while (*fork == 0)
	{
		pthread_mutex_unlock(mutex);
		if (death_check(philo) == 1)
			return (-1);
		usleep(100);
		pthread_mutex_lock(mutex);
	}
	*fork = 0;
	pthread_mutex_unlock(mutex);
	if (print_lock(philo, "has taken a fork\n") == -1)
		return (-1);
	return (0);
}

int	eating_routine(t_phidata *philo)
{
	struct timeval	time;

	if (philo->id % 2)
	{
		fork_taking(philo, &philo->leftfork_mutex, &philo->leftfork);
		// pthread_mutex_lock(&philo->leftfork_mutex);
		// while (philo->leftfork == 0)
		// {
		// 	pthread_mutex_unlock(&philo->leftfork_mutex);
		// 	if (death_check(philo) == 1)
		// 		return (-1);
		// 	usleep(100);
		// 	pthread_mutex_lock(&philo->leftfork_mutex);
		// }
		// philo->leftfork = 0;
		// pthread_mutex_unlock(&philo->leftfork_mutex);
		// if (print_lock(philo, "has taken a fork\n") == -1)
		// 	return (-1);
		fork_taking(philo, philo->rightfork_mutex, philo->rightfork);
		// pthread_mutex_lock(philo->rightfork_mutex);
		// while (*philo->rightfork == 0)
		// {
		// 	pthread_mutex_unlock(philo->rightfork_mutex);
		// 	if (death_check(philo) == 1)
		// 		return (-1);
		// 	usleep(100);
		// 	pthread_mutex_lock(philo->rightfork_mutex);
		// }
		// *philo->rightfork = 0;
		// pthread_mutex_unlock(philo->rightfork_mutex);
		// if (print_lock(philo, "has taken a fork\n") == -1)
		// 	return (-1);
	}
	else
	{
		fork_taking(philo, philo->rightfork_mutex, philo->rightfork);
		fork_taking(philo, &philo->leftfork_mutex, &philo->leftfork);
		// pthread_mutex_lock(philo->rightfork_mutex);
		// while (*philo->rightfork == 0)
		// {
		// 	pthread_mutex_unlock(philo->rightfork_mutex);
		// 	if (death_check(philo) == 1)
		// 		return (-1);
		// 	usleep(100);
		// 	pthread_mutex_lock(philo->rightfork_mutex);
		// }
		// *philo->rightfork = 0;
		// pthread_mutex_lock(&philo->leftfork_mutex);
		// while (philo->leftfork == 0)
		// {
		// 	pthread_mutex_unlock(&philo->leftfork_mutex);
		// 	if (death_check(philo) == 1)
		// 		return (-1);
		// 	usleep(100);
		// 	pthread_mutex_lock(&philo->leftfork_mutex);
		// }
		// philo->leftfork = 0;
		// pthread_mutex_unlock(&philo->leftfork_mutex);
		// if (print_lock(philo, "has taken a fork\n") == -1)
		// 	return (-1);
	}
	if (print_lock(philo, "is eating\n") == -1)
		return (-1);
	if (ft_wait(philo, philo->data->tteat) == -1)
		return (-1);
	pthread_mutex_lock(&philo->leftfork_mutex);
	philo->leftfork = 1;
	pthread_mutex_unlock(&philo->leftfork_mutex);
	pthread_mutex_lock(philo->rightfork_mutex);
	*philo->rightfork = 1;
	pthread_mutex_unlock(philo->rightfork_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->nbmeal++;
	gettimeofday(&time, NULL);
	philo->lastmeal = get_time_ms(time);
	pthread_mutex_unlock(&philo->meal_mutex);
	usleep(100);
	return (0);
}

int	sleeping_routine(t_phidata *philo)
{
	if (death_check(philo) == 1)
		return (-1);
	print_lock(philo, "is sleeping\n");
	if (ft_wait(philo, philo->data->ttsleep) == -1)
		return (-1);
	if (death_check(philo) == 1)
		return (-1);
	print_lock(philo, "is thinking\n");
	usleep(100);
	// usleep(10);
	// if (death_check(philo) == 1)
	// 	return (-1);
	return (0);
}

bool	death_check(t_phidata *philo)
{
	pthread_mutex_lock(&philo->data->state_mutex);
	if (philo->data->state == STOP)
	{
		pthread_mutex_unlock(&philo->data->state_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->state_mutex);
	return (0);
}

void	*routine(void *arg)
{
	t_phidata	*philo;

	philo = (t_phidata *)arg;
	// print_lock(philo, "is waiting to start\n");
	pthread_mutex_lock(&philo->data->start_mutex);
	while (philo->data->start_flag == 0)
	{
		pthread_mutex_unlock(&philo->data->start_mutex);
		usleep(1);
		pthread_mutex_lock(&philo->data->start_mutex);
	}
	pthread_mutex_unlock(&philo->data->start_mutex);
	// print_lock(philo, "begins routine\n");
	print_lock(philo, "is thinking\n");
	if (philo->id % 2 == 0)
		usleep(200);
	while (1)
	{
		// if (death_check(philo) == 1)
		// 	return (NULL);
		if (eating_routine(philo) == -1)
			return (NULL);
		// if (death_check(philo) == 1)
		// 	return (NULL);
		if (sleeping_routine(philo) == -1)
			return (NULL);
		usleep(100);
	}
	return (NULL);
}

bool	is_dead(t_phidata *philo, int i)
{
	struct timeval	current_time;

	// print_lock(&philo[i], "death check\n");
	gettimeofday(&current_time, NULL);
	if (philo[i].lastmeal != 0)
	{
		if ((get_time_ms(current_time)
				- philo[i].lastmeal) >= philo[i].data->ttdie)
		{
			print_lock(&philo[i], "died\n");
			return (1);
		}
	}
	else
	{
		if ((get_time_ms(current_time)
				- philo[i].data->start_time) >= philo[i].data->ttdie)
		{
			print_lock(&philo[i], "died\n");
			return (1);
		}
	}
	return (0);
}

void	*threads_checking(void *arg)
{
	t_phidata	*philo;
	int			i;

	philo = (t_phidata *)arg;
	i = 0;
	while (1)
	{
		while (i < philo->data->nbphilo)
		{
			pthread_mutex_lock(&philo[i].meal_mutex);
			if ((philo[i].data->maxmeal != -1
					&& philo[i].nbmeal >= philo[i].data->maxmeal)
				|| is_dead(philo, i) == 1)
			{
				pthread_mutex_unlock(&philo[i].meal_mutex);
				pthread_mutex_lock(&philo->data->state_mutex);
				philo[i].data->state = STOP;
				pthread_mutex_unlock(&philo->data->state_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&philo[i].meal_mutex);
			i++;
			// usleep(100);
		}
		usleep(100);
		i = 0;
	}
	return (NULL);
}

int	monitor_thread(t_phidata *philo)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, threads_checking, philo) != 0)
	{
		free_philos(philo, philo->data->nbphilo);
		return (handle_error("[Error] : phtread_create crashed\n", NULL, -1));
	}
	// print_lock(philo, "")
	pthread_detach(monitor);
	return (0);
}

int	main(int argc, char **argv)
{
	t_phidata		*philo;
	struct timeval	start;
	t_data			*data;
	int				i;

	i = 0;
	if (argc < 5 || argc > 6)
		return (handle_error("There needs to be 4 or 5 arguments\n", NULL, 1));
	if (check_arg(argv) == -1)
		return (2);
	philo = ft_calloc(ft_atoi(argv[1]), sizeof(t_phidata));
	if (!philo)
		return (handle_error("[Error] : Malloc failure\n", NULL, 2));
	data = get_data(argv);
	if (!data)
	{
		free(philo);
		return (2);
	}
	if (init_loop(philo, data) == -1)
		return (2);
	// usleep(1000);
	pthread_mutex_lock(&data->start_mutex);
	gettimeofday(&start, NULL);
	data->start_time = get_time_ms(start);
	data->start_flag = 1;
	pthread_mutex_unlock(&data->start_mutex);
	// printf("start routine unlocked\n");
	monitor_thread(philo);
	while (i < data->nbphilo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	free_philos(philo, philo->data->nbphilo);
	return (0);
}
