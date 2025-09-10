/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucasp <lucasp@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:52:13 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/10 14:13:21 by lucasp           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

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
		if (argv[i][0] == '\0')
		{
			handle_error("one or several args are empty\n", NULL, 1);
			return (-1);
		}
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
