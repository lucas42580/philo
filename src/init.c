/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:49:30 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/23 13:40:11 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	init_philo(t_phidata *philo, int i)
{
	philo[i].id = i + 1;
	philo[i].leftfork = 1;
	if (i + 1 < philo->data->nbphilo)
	{
		philo[i].rightfork_mutex = &philo[i + 1].leftfork_mutex;
		philo[i].rightfork = &philo[i + 1].leftfork;
	}
	else if (i + 1 == philo->data->nbphilo && philo->data->nbphilo != 1)
	{
		philo[i].rightfork_mutex = &philo[0].leftfork_mutex;
		philo[i].rightfork = &philo[0].leftfork;
	}
	else
	{
		philo[i].one_philo = 1;
		philo[i].rightfork_mutex = &philo[i].leftfork_mutex;
		philo[i].rightfork = 0;
	}
	if (mutex_init(philo, i) == -1)
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
	data->nbphilo = ft_atoi(argv[1]);
	data->ttdie = ft_atoi(argv[2]);
	data->tteat = ft_atoi(argv[3]);
	data->ttsleep = ft_atoi(argv[4]);
	if (argv[5])
		data->maxmeal = ft_atoi(argv[5]);
	else
		data->maxmeal = -1;
	if (data_mutex_init(data) == -1)
		return (NULL);
	return (data);
}

static void	wait_for_philos(t_phidata *philo, int stop)
{
	int	i;

	i = 0;
	while (i < stop)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	free_philos(philo, stop);
	return ;
}

static int	create_and_launch_threads(t_data *data, t_phidata *philo)
{
	int				i;
	struct timeval	start;

	i = 0;
	while (i < data->nbphilo)
	{
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]) != 0)
		{
			ft_putstr_fd("[ERROR] : pthread_create_crashed\n", 2);
			gettimeofday(&start, NULL);
			data->start_time = get_time_ms(start);
			pthread_mutex_lock(&data->state_mutex);
			data->state = STOP;
			pthread_mutex_unlock(&data->state_mutex);
			pthread_mutex_unlock(&data->start_mutex);
			wait_for_philos(philo, i);
		}
		i++;
	}
	return (0);
}

int	init_loop(t_phidata *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbphilo)
	{
		philo[i].data = data;
		if (init_philo(philo, i) == -1)
			return (-1);
		i++;
	}
	pthread_mutex_lock(&data->start_mutex);
	if (create_and_launch_threads(data, philo) == -1)
		return (-1);
	return (0);
}
