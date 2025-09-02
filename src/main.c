/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:37:00 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/02 16:12:09 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	print_lock(t_phidata *philo, char *str)
{
	long			time_ms;
	struct timeval	time;

	pthread_mutex_lock(&philo->print_mutex);
	gettimeofday(&time, NULL);
	time_ms = get_time_ms(time) - philo->data->start_time;
	printf("%ld %d %s", time_ms, philo->id, str);
	pthread_mutex_unlock(&philo->print_mutex);
}

void	eating_routine(t_phidata *philo)
{
	static int		i;
	struct timeval	start;
	struct timeval	end;
	long			time;

	i = 1;
	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	if (philo->leftfork == 1 && philo->rightfork == 1)
	{
		time = get_time_ms(end) - philo->data->start_time;
		print_lock(philo, "is eating\n");
		// usleep(philo->data->tteat);
		while (get_duration(start, end) < philo->data->tteat)
			gettimeofday(&end, NULL);
		philo->nbmeal++;
	}
	usleep(300);
	return ;
}

void	sleeping_routine(t_phidata *philo)
{
	struct timeval	start;
	struct timeval	end;
	long			time;

	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	time = get_time_ms(end) - philo->data->start_time;
	print_lock(philo, "is sleeping\n");
	while (get_duration(start, end) < philo->data->ttsleep)
		gettimeofday(&end, NULL);
	// usleep(philo->data->ttsleep);
	gettimeofday(&end, NULL);
	time = get_time_ms(end) - philo->data->start_time;
	print_lock(philo, "is thinking\n");
	usleep(300);
	return ;
}

void	*routine(void *arg)
{
	t_phidata		*philo;
	pthread_mutex_t	start_mutex;

	philo = (t_phidata *)arg;
	philo->rightfork = 1;
	pthread_mutex_init(&start_mutex, NULL);
	// print_lock(philo, "is waiting to start\n");
	// usleep(1000);
	pthread_mutex_lock(&start_mutex);
	while (philo->data->start_flag == 0)
		usleep(1);
	pthread_mutex_unlock(&start_mutex);
	print_lock(philo, "begins routine\n");
	// usleep(10);
	while (philo->nbmeal < philo->data->maxmeal)
	{
		eating_routine(philo);
		sleeping_routine(philo);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_phidata		*philo;
	struct timeval	start;
	t_data			*data;
	pthread_mutex_t	start_mutex;
	int				i;

	i = 0;
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
	if (init_loop(philo, data, argv) == -1)
		return (2);
	pthread_mutex_init(&start_mutex, NULL);
	pthread_mutex_lock(&start_mutex);
	usleep(1000);
	gettimeofday(&start, NULL);
	data->start_time = get_time_ms(start);
	data->start_flag = 1;
	// printf("start routine unlocked\n");
	pthread_mutex_unlock(&start_mutex);
	while (i < ft_atoi(argv[1]))
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	free_philos(philo, philo->data->nbphilo);
	return (0);
}
