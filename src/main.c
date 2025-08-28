/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:37:00 by lpaysant          #+#    #+#             */
/*   Updated: 2025/08/28 17:20:15 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	get_duration(struct timeval start, struct timeval end)
{
	long	start_mil;
	long	end_mil;

	start_mil = start.tv_sec * 1000 + start.tv_usec / 1000;
	end_mil = end.tv_sec * 1000 + end.tv_usec / 1000;
	return (end_mil - start_mil);
}

void	get_data(char **argv, t_phidata *philo)
{
	philo->data = ft_calloc(1, sizeof(t_data));
	philo->data->nbphilo = ft_atoi(argv[1]);
	philo->data->ttdie = ft_atoi(argv[2]);
	philo->data->tteat = ft_atoi(argv[3]);
	philo->data->ttsleep = ft_atoi(argv[4]);
	if (argv[5])
		philo->data->maxmeal = ft_atoi(argv[5]);
	else
		philo->data->maxmeal = 0;
	philo->data->start = 0;
	if (philo->data->nbphilo % 2 == 0)
		philo->data->is_even = 1;
	else
		philo->data->is_even = 0;
}

static int	is_number(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
		{
			handle_error("[Error] : All args must be positive integers\n", NULL,
				1);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	check_arg(char **argv)
{
	int	i;
	int	nb;

	i = 1;
	while (argv[i])
	{
		if (is_number(argv[i]) == -1)
			return (-1);
		nb = ft_atoi(argv[i]);
		if ((i == 1) && (nb < 0 || nb > 200))
		{
			handle_error("number of philos must be between 1 and 200\n", NULL,
				1);
			return (-1);
		}
		if (i > 1 && nb < 0)
		{
			handle_error("all args must be positive ints\n", NULL, 1);
			return (-1);
		}
		i++;
	}
	return (0);
}

void	eating_routine(t_phidata *philo)
{
	int				i;
	struct timeval	start;
	struct timeval	end;

	i = 0;
	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	if (philo->nbfork == 2)
	{
		while (get_duration(start, end) < philo->data->tteat)
			gettimeofday(&end, NULL);
		philo->nbmeal++;
		printf("philo number %d has eaten\n", philo->id);
	}
	return ;
}

void	sleeping_routine(t_phidata *philo)
{
	struct timeval	start;
	struct timeval	end;
	int				i;

	i = 0;
	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	while (get_duration(start, end) < philo->data->ttsleep)
		gettimeofday(&end, NULL);
	printf("philo number %d has slept\n", philo->id);
	return ;
}

void	*routine(void *arg)
{
	t_phidata	*philo;

	philo = (t_phidata *)arg;
	philo->nbfork = 2;
	while (philo->nbmeal < philo->data->maxmeal)
	{
		eating_routine(philo);
		sleeping_routine(philo);
	}
	printf("philo number %d ate all its meals\n", philo->id);
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
		return (-1);
	philo = malloc(ft_atoi(argv[1]) * sizeof(t_phidata));
	if (!philo)
		return (handle_error("[Error] : Malloc failure\n", NULL, 2));
	while (i < ft_atoi(argv[1]))
	{
		printf("i = %d\n", i);
		philo[i].id = i + 1;
		philo[i].nbfork = 1;
		philo[i].data = NULL;
		get_data(argv, &philo[i]);
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]) != 0)
			return (handle_error("[Error] : phtread_create crashed\n", philo,
					2));
		i++;
	}
	usleep(100);
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	return (0);
}
