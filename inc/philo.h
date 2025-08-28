/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:41:58 by lpaysant          #+#    #+#             */
/*   Updated: 2025/08/28 17:10:51 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum e_fork
{
	LEFT,
	RIGHT
}				t_fork;

typedef enum e_status
{
	THINK,
	EAT,
	SLEEP,
	DEAD
}				t_status;
typedef struct s_data
{
	long		start;
	int			nbphilo;
	int			is_even;
	int			ttdie;
	int			tteat;
	int			ttsleep;
	int			maxmeal;
}				t_data;
typedef struct s_phidata
{
	pthread_t	thread;
	int			id;
	int			nbmeal;
	int			state;
	int			nbfork;
	char		*forktab;
	t_data		*data;
}				t_phidata;
int				main(int argc, char **argv);
int				check_arg(char **argv);

int				ft_atoi(const char *nptr);
void			ft_putstr_fd(char *str, int fd);
int				ft_strlen(const char *str);
int				handle_error(char *str, t_phidata *philo, int err);
void			*ft_calloc(size_t nmemb, size_t size);
