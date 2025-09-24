/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:17:14 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/24 19:04:49 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	sleeping_routine(t_phidata *philo)
{
	print_lock(philo, "is sleeping\n", 0);
	if (ft_wait(philo, philo->data->ttsleep) == -1)
		return (-1);
	print_lock(philo, "is thinking\n", 0);
	if (philo->data->nbphilo % 2 != 0
		&& philo->data->ttsleep <= philo->data->tteat)
		ft_wait(philo, philo->data->tteat);
	return (0);
}

static int	eat_and_update(t_phidata *philo)
{
	struct timeval	time;

	if (print_lock(philo, "is eating\n", 0) == -1)
		return (-1);
	pthread_mutex_lock(&philo->meal_mutex);
	gettimeofday(&time, NULL);
	philo->lastmeal = get_time_ms(time);
	pthread_mutex_unlock(&philo->meal_mutex);
	if (ft_wait(philo, philo->data->tteat) == -1)
		return (-1);
	pthread_mutex_lock(&philo->leftfork_mutex);
	philo->leftfork = true;
	pthread_mutex_unlock(&philo->leftfork_mutex);
	pthread_mutex_lock(philo->rightfork_mutex);
	*philo->rightfork = true;
	pthread_mutex_unlock(philo->rightfork_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->nbmeal++;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (0);
}

static int	fork_taking(t_phidata *philo, pthread_mutex_t *mutex, bool *fork)
{
	pthread_mutex_lock(mutex);
	while (*fork == false)
	{
		pthread_mutex_unlock(mutex);
		if (death_check(philo) == true)
			return (-1);
		usleep(400);
		pthread_mutex_lock(mutex);
	}
	*fork = false;
	pthread_mutex_unlock(mutex);
	if (print_lock(philo, "has taken a fork\n", 0) == -1)
		return (-1);
	return (0);
}

static int	eating_routine(t_phidata *philo)
{
	if (philo->id % 2)
	{
		if (fork_taking(philo, &philo->leftfork_mutex, &philo->leftfork) == -1)
			return (-1);
		if (philo->one_philo == true)
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

void	*routine(void *arg)
{
	t_phidata	*philo;

	philo = (t_phidata *)arg;
	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);
	if (death_check(philo) == true)
		return (NULL);
	print_lock(philo, "is thinking\n", 0);
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
