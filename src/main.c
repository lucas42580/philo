/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:37:00 by lpaysant          #+#    #+#             */
/*   Updated: 2025/05/09 17:03:51 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	get_data(char **argv, t_data *data)
{
	data->nbphilo = ft_atoi(argv[1]);
	data->ttdie = ft_atoi(argv[2]);
	data->tteat = ft_atoi(argv[3]);
	data->ttsleep = ft_atoi(argv[4]);
	if (argv[5])
		data->maxmeal = ft_atoi(argv[5]);
	else
		data->maxmeal = 0;
	data->start = 0;
}

static int	is_number(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
		{
			handle_error("[Error] : All args must be positive integers\n",
				NULL);
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
			handle_error("number of philos must be between 1 and 200\n", NULL);
			return (-1);
		}
		if (i > 1 && nb < 0)
		{
			handle_error("all args must be positive ints\n", NULL);
			return (-1);
		}
		i++;
	}
	return (0);
}

void	*routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (data->start == 0)
		;
	printf("all threads created\n");
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	i = 0;
	if (argc < 5 || argc > 6)
		handle_error("There needs to be 4 or 5 arguments\n", NULL);
	if (check_arg(argv) == -1)
		return (-1);
	get_data(argv, &data);
	printf("nbphilo = %d\n", data.nbphilo);
	data.philo = malloc(data.nbphilo * sizeof(t_phidata));
	if (!data.philo)
		handle_error("[Error] : Malloc failure\n", NULL);
	while (i < data.nbphilo)
	{
		printf("i = %d\n", i);
		data.philo[i].id = i + 1;
		data.philo[i].nbfork = 1;
		if (pthread_create(&data.philo[i].thread, NULL, routine, &data) != 0)
			handle_error("[Error] : phtread_create crashed\n", &data);
		i++;
	}
	usleep(100);
	data.start = 1;
	printf("ici\n");
	i = 0;
	while (i < data.nbphilo)
	{
		pthread_join(data.philo[i].thread, NULL);
		i++;
	}
	return (0);
}
