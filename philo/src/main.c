/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:37:00 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/24 17:22:19 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	start_and_wait_philos(t_phidata *philo, t_data *data)
{
	int				i;
	struct timeval	start;

	gettimeofday(&start, NULL);
	data->start_time = get_time_ms(start);
	pthread_mutex_unlock(&data->start_mutex);
	threads_checking(philo);
	i = 0;
	while (i < data->nbphilo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_phidata	*philo;
	t_data		*data;

	if (argc < 5 || argc > 6)
		return (handle_error("There needs to be 4 or 5 arguments\n", NULL, 1));
	if (check_arg(argv) == -1)
		return (1);
	philo = ft_calloc(ft_atoi(argv[1]), sizeof(t_phidata));
	if (!philo)
		return (handle_error("[Error] : Malloc failure\n", NULL, 2));
	data = get_data(argv);
	if (!data)
	{
		free(philo);
		return (1);
	}
	if (init_loop(philo, data) == -1)
		return (1);
	start_and_wait_philos(philo, data);
	free_philos(philo, philo->data->nbphilo);
	return (0);
}
