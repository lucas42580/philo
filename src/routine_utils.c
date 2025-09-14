/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 12:05:15 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/14 15:30:10 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

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
	(void)philo;
	while (get_time_ms(end) - get_time_ms(start) < time)
	{
		if (death_check(philo) == 1)
			return (-1);
		gettimeofday(&end, NULL);
		usleep(600);
	}
	return (0);
}
