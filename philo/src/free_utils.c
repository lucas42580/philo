/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:15:57 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/24 12:56:44 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	handle_error(char *str, t_phidata *philo, int err)
{
	if (philo != NULL)
	{
		if (philo->data)
			free(philo->data);
	}
	ft_putstr_fd(str, 2);
	return (err);
}

void	free_datas(t_phidata *philo, int nbphilo)
{
	int	i;

	i = 0;
	while (i < nbphilo)
	{
		if (philo[i].data)
		{
			free(philo[i].data);
			philo[i].data = NULL;
		}
		i++;
	}
}

void	free_philos(t_phidata *philo, int nbphilo)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&philo->data->print_mutex);
	pthread_mutex_destroy(&philo->data->state_mutex);
	pthread_mutex_destroy(&philo->data->start_mutex);
	free(philo->data);
	while (i < nbphilo)
	{
		mutex_destroyer(&philo[i], 2);
		i++;
	}
	free(philo);
}

int	mutex_destroyer(t_phidata *philo, int i)
{
	pthread_mutex_destroy(&philo->meal_mutex);
	if (i > 0)
		pthread_mutex_destroy(&philo->leftfork_mutex);
	return (-1);
}
