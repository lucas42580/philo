/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:41:58 by lpaysant          #+#    #+#             */
/*   Updated: 2025/05/09 15:00:19 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum e_status
{
	THINK = 0,
	EAT = 1,
	SLEEP = 2,
	DEAD = 3
}				t_status;
typedef struct s_phidata
{
	pthread_t	thread;
	int			id;
	int			nbeat;
	int			state;
	int			nbfork;
	t_status	status;
}				t_phidata;
typedef struct s_data
{
	int			start;
	int			nbphilo;
	int			ttdie;
	int			tteat;
	int			ttsleep;
	int			maxmeal;
	t_phidata	*philo;
}				t_data;
int				main(int argc, char **argv);
int				check_arg(char **argv);

int				ft_atoi(const char *nptr);
void			ft_putstr_fd(char *str, int fd);
int				ft_strlen(const char *str);
void			handle_error(char *str, t_data *data);
