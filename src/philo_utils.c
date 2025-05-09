/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:40:19 by lpaysant          #+#    #+#             */
/*   Updated: 2025/05/09 11:55:45 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	handle_error(char *str, t_data *data)
{
	int	i;

	i = 0;
	if (data != NULL)
	{
		if ((*data).philo)
			free((*data).philo);
	}
	ft_putstr_fd(str, 2);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

static int	getnbr(const char *str, int i, int sign)
{
	int	nb;

	nb = 0;
	while (str[i] >= 48 && str[i] <= 57)
	{
		nb = nb * 10 + (str[i] - '0');
		if ((nb * sign) > INT_MAX || (nb * sign) < INT_MIN)
			return (-1);
		i++;
	}
	nb = nb * sign;
	return (nb);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	if (ft_strlen(nptr) > 10)
		return (-1);
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ' || nptr[i] == '+'
		|| nptr[i] == '-' || (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		if ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
			i++;
		if (nptr[i] == '+' || nptr[i] == '-')
		{
			if (nptr[i] == '-' && sign > 0)
				sign = sign * -1;
			i++;
			if (!(nptr[i] >= '0' && nptr[i] <= '9'))
				return (0);
		}
		if (nptr[i] >= '0' && nptr[i] <= '9')
			return (getnbr(nptr, i, sign));
	}
	return (0);
}
