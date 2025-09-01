/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:37:00 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/01 16:10:49 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

// void	print(t_phidata *philo, char *str)
// {
// }

void	eating_routine(t_phidata *philo)
{
	int				i;
	struct timeval	start;
	struct timeval	end;
	long			time;

	i = 0;
	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	if (philo->leftfork == 1 && philo->rightfork == 1)
	{
		time = get_time_ms(end) - philo->data->begin;
		printf("%ld %d is eating\n", time, philo->id);
		while (get_duration(start, end) < philo->data->tteat)
			gettimeofday(&end, NULL);
		philo->nbmeal++;
	}
	return ;
}

void	sleeping_routine(t_phidata *philo)
{
	struct timeval	start;
	struct timeval	end;
	long			time;

	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	time = get_time_ms(end) - philo->data->begin;
	printf("%ld %d is sleeping\n", time, philo->id);
	while (get_duration(start, end) < philo->data->ttsleep)
		gettimeofday(&end, NULL);
	gettimeofday(&end, NULL);
	time = get_time_ms(end) - philo->data->begin;
	printf("%ld %d is thinking\n", time, philo->id);
	return ;
}

void	*routine(void *arg)
{
	t_phidata		*philo;
	struct timeval	start;

	philo = (t_phidata *)arg;
	philo->rightfork = 1;
	gettimeofday(&start, NULL);
	while (get_time_ms(start) - philo->data->begin < 10)
		;
	while (philo->nbmeal < philo->data->maxmeal)
	{
		eating_routine(philo);
		sleeping_routine(philo);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_phidata	*philo;
	int			i;

	i = 0;
	if (argc < 5 || argc > 6)
		return (handle_error("There needs to be 4 or 5 arguments\n", NULL, 1));
	if (check_arg(argv) == -1)
		return (2);
	philo = ft_calloc(ft_atoi(argv[1]), sizeof(t_phidata));
	if (!philo)
		return (handle_error("[Error] : Malloc failure\n", NULL, 2));
	if (init_loop(philo, argv) == -1)
		return (2);
	usleep(100);
	while (i < ft_atoi(argv[1]))
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	free_philos(philo, philo->data->nbphilo);
	return (0);
}
