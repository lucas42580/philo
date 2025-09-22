/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 11:56:52 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/22 17:02:52 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

bool	is_dead(t_phidata *philo, int i)
{
	struct timeval	current_time;

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

int	check_philos(t_phidata *philo, int *i, int *has_eaten)
{
	pthread_mutex_lock(&philo[*i].meal_mutex);
	if (philo[*i].data->maxmeal != -1
		&& philo[*i].nbmeal >= philo[*i].data->maxmeal)
	{
		pthread_mutex_unlock(&philo[*i].meal_mutex);
		(*i)++;
		(*has_eaten)++;
	}
	else if (*i < philo->data->nbphilo && (is_dead(philo, *i) == 1
			|| *has_eaten >= philo->data->nbphilo))
	{
		pthread_mutex_unlock(&philo[*i].meal_mutex);
		pthread_mutex_lock(&philo->data->state_mutex);
		philo->data->state = STOP;
		pthread_mutex_unlock(&philo->data->state_mutex);
		return (-1);
	}
	else
	{
		pthread_mutex_unlock(&philo[*i].meal_mutex);
		(*i)++;
	}
	usleep(100);
	return (0);
}

void	threads_checking(t_phidata *philo)
{
	int	i;
	int	has_eaten;

	i = 0;
	has_eaten = 0;
	while (1)
	{
		while (i < philo->data->nbphilo)
		{
			if (check_philos(philo, &i, &has_eaten) == -1)
				return ;
		}
		if (has_eaten >= philo->data->nbphilo)
		{
			pthread_mutex_lock(&philo->data->state_mutex);
			philo->data->state = STOP;
			pthread_mutex_unlock(&philo->data->state_mutex);
			return ;
		}
		i = 0;
		has_eaten = 0;
	}
	return ;
}
