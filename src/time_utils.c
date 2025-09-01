/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:19:38 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/01 13:20:27 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

long	get_time_ms(struct timeval time)
{
	long	time_ms;

	time_ms = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (time_ms);
}

long	get_duration(struct timeval start, struct timeval end)
{
	long	start_mil;
	long	end_mil;

	start_mil = get_time_ms(start);
	end_mil = get_time_ms(end);
	return (end_mil - start_mil);
}
