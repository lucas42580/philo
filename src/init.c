/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:49:30 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/02 16:15:02 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	mutex_init(t_phidata *philo)
{
	if (pthread_mutex_init(&philo->lastmeal_mutex, NULL) != 0)
		return (mutex_destroyer(philo, 0));
	if (pthread_mutex_init(&philo->leftfork_mutex, NULL) != 0)
		return (mutex_destroyer(philo, 1));
	if (pthread_mutex_init(&philo->rightfork_mutex, NULL) != 0)
		return (mutex_destroyer(philo, 2));
	if (pthread_mutex_init(&philo->nbmeal_mutex, NULL) != 0)
		return (mutex_destroyer(philo, 3));
	if (pthread_mutex_init(&philo->state_mutex, NULL) != 0)
		return (mutex_destroyer(philo, 4));
	if (pthread_mutex_init(&philo->print_mutex, NULL) != 0)
		return (mutex_destroyer(philo, 5));
	return (0);
}

int	init_philo(t_phidata *philo, int i)
{
	philo[i].id = i + 1;
	philo[i].leftfork = 1;
	philo[i].rightfork = 0;
	// philo->data = NULL;
	if (mutex_init(philo) == -1)
	{
		free_philos(philo, i);
		return (-1);
	}
	return (0);
}

t_data	*get_data(char **argv)
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
	{
		handle_error("[Error] : Malloc failure\n", NULL, 2);
		return (NULL);
	}
	data->start_time = 0;
	data->nbphilo = ft_atoi(argv[1]);
	data->ttdie = ft_atoi(argv[2]);
	data->tteat = ft_atoi(argv[3]);
	data->ttsleep = ft_atoi(argv[4]);
	if (argv[5])
		data->maxmeal = ft_atoi(argv[5]);
	else
		data->maxmeal = 0;
	if (data->nbphilo % 2 == 0)
		data->is_even = 1;
	else
		data->is_even = 0;
	return (data);
}

int	init_loop(t_phidata *philo, t_data *data, char **argv)
{
	int	i;

	// struct timeval	start;
	// long			start_time;
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		if (init_philo(philo, i) == -1)
			return (-1);
		philo[i].data = data;
		i++;
	}
	i = 0;
	// gettimeofday(&start, NULL);
	// start_time = get_time_ms(start);
	while (i < ft_atoi(argv[1]))
	{
		// philo[i].data->start_time = start_time;
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]) != 0)
			return (handle_error("[Error] : phtread_create crashed\n", philo,
					-1));
		i++;
	}
	return (0);
}
