/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:37:00 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/14 17:04:11 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	fork_taking(t_phidata *philo, pthread_mutex_t *mutex, bool *fork)
{
	pthread_mutex_lock(mutex);
	while (*fork == false)
	{
		pthread_mutex_unlock(mutex);
		if (death_check(philo) == 1)
			return (-1);
		usleep(400);
		// if (ft_wait(philo, 400) == -1)
		// 	return (-1);
		pthread_mutex_lock(mutex);
	}
	*fork = false;
	pthread_mutex_unlock(mutex);
	if (print_lock(philo, "has taken a fork\n") == -1)
		return (-1);
	return (0);
}

int	eat_and_update(t_phidata *philo)
{
	struct timeval	time;

	if (print_lock(philo, "is eating\n") == -1)
		return (-1);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->nbmeal++;
	gettimeofday(&time, NULL);
	philo->lastmeal = get_time_ms(time);
	pthread_mutex_unlock(&philo->meal_mutex);
	if (ft_wait(philo, philo->data->tteat) == -1)
		return (-1);
	pthread_mutex_lock(&philo->leftfork_mutex);
	philo->leftfork = 1;
	pthread_mutex_unlock(&philo->leftfork_mutex);
	pthread_mutex_lock(philo->rightfork_mutex);
	*philo->rightfork = 1;
	pthread_mutex_unlock(philo->rightfork_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->data->maxmeal != -1 && philo->nbmeal >= philo->data->maxmeal)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (0);
}

int	eating_routine(t_phidata *philo)
{
	if (philo->id % 2)
	{
		if (fork_taking(philo, &philo->leftfork_mutex, &philo->leftfork) == -1)
			return (-1);
		if (philo->one_philo == 1)
		{
			if (ft_wait(philo, philo->data->ttdie + 100) == -1)
				return (-1);
		}
		if (fork_taking(philo, philo->rightfork_mutex, philo->rightfork) == -1)
			return (-1);
	}
	else
	{
		if (fork_taking(philo, philo->rightfork_mutex, philo->rightfork) == -1)
			return (-1);
		if (fork_taking(philo, &philo->leftfork_mutex, &philo->leftfork) == -1)
			return (-1);
	}
	if (eat_and_update(philo) == -1)
		return (-1);
	return (0);
}

int	sleeping_routine(t_phidata *philo)
{
	print_lock(philo, "is sleeping\n");
	if (ft_wait(philo, philo->data->ttsleep) == -1)
		return (-1);
	print_lock(philo, "is thinking\n");
	if (philo->data->nbphilo % 2 != 0
		&& philo->data->ttsleep <= philo->data->tteat)
		ft_wait(philo, philo->data->tteat);
	return (0);
}

void	*routine(void *arg)
{
	t_phidata	*philo;

	philo = (t_phidata *)arg;
	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);
	print_lock(philo, "is thinking\n");
	if (philo->id % 2 == 0)
		if (ft_wait(philo, philo->data->tteat) == -1)
			return (NULL);
	while (1)
	{
		if (eating_routine(philo) == -1)
			return (NULL);
		if (sleeping_routine(philo) == -1)
			return (NULL);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_phidata		*philo;
	struct timeval	start;
	t_data			*data;
	int				i;

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
	gettimeofday(&start, NULL);
	data->start_time = get_time_ms(start);
	pthread_mutex_unlock(&data->start_mutex);
	// monitor_thread(philo, monitor);
	threads_checking(philo);
	i = 0;
	// pthread_join(philo->data->monitor, NULL);
	while (i < data->nbphilo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	free_philos(philo, philo->data->nbphilo);
	return (0);
}
