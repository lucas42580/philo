/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:35:27 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/23 14:27:42 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	data_mutex_init(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		ft_putstr_fd("[ERROR] : pthread_mutex_init failed\n", 2);
		free(data);
		return (-1);
	}
	if (pthread_mutex_init(&data->state_mutex, NULL) != 0)
	{
		ft_putstr_fd("[ERROR] : pthread_mutex_init failed\n", 2);
		pthread_mutex_destroy(&data->print_mutex);
		free(data);
		return (-1);
	}
	if (pthread_mutex_init(&data->start_mutex, NULL) != 0)
	{
		ft_putstr_fd("[ERROR] : pthread_mutex_init failed\n", 2);
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->state_mutex);
		free(data);
		return (-1);
	}
	return (0);
}

int	philo_mutex_init(t_phidata *philo, int i)
{
	if (pthread_mutex_init(&philo[i].meal_mutex, NULL) != 0)
	{
		ft_putstr_fd("[ERROR] : pthread_mutex_init failed\n", 2);
		mutex_destroyer(philo, 0);
		return (-1);
	}
	if (pthread_mutex_init(&philo[i].leftfork_mutex, NULL) != 0)
	{
		ft_putstr_fd("[ERROR] : pthread_mutex_init failed\n", 2);
		mutex_destroyer(philo, 1);
		return (-1);
	}
	return (0);
}
