/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:15:57 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/02 13:34:04 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	handle_error(char *str, t_phidata *philo, int err)
{
	int	i;

	i = 0;
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
	// free_datas(philo, nbphilo);
	free(philo->data);
	while (i < nbphilo)
	{
		mutex_destroyer(&philo[i], 4);
		i++;
	}
	free(philo);
}

int	mutex_destroyer(t_phidata *philo, int i)
{
	pthread_mutex_destroy(&philo->lastmeal_mutex);
	if (i > 0)
		pthread_mutex_destroy(&philo->leftfork_mutex);
	if (i > 1)
		pthread_mutex_destroy(&philo->rightfork_mutex);
	if (i > 2)
		pthread_mutex_destroy(&philo->nbmeal_mutex);
	if (i > 3)
		pthread_mutex_destroy(&philo->state_mutex);
	if (i > 4)
		pthread_mutex_destroy(&philo->print_mutex);
	return (-1);
}
