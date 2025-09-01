/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:41:58 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/01 15:04:49 by lpaysant         ###   ########.fr       */
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
}					t_fork;

typedef enum e_status
{
	THINK,
	EAT,
	SLEEP,
	DEAD
}					t_status;
typedef struct s_data
{
	long			begin;
	int				nbphilo;
	int				is_even;
	int				ttdie;
	int				tteat;
	int				ttsleep;
	int				maxmeal;
}					t_data;
typedef struct s_phidata
{
	pthread_t		thread;
	int				id;
	int				nbmeal;
	int				state;
	long			lastmeal;
	int				leftfork;
	int				rightfork;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	lastmeal_mutex;
	pthread_mutex_t	nbmeal_mutex;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	leftfork_mutex;
	pthread_mutex_t	rightfork_mutex;
	t_data			*data;
}					t_phidata;

int					main(int argc, char **argv);
int					get_data(char **argv, t_phidata *philo);
int					init_loop(t_phidata *philo, char **argv);
int					init_philo(t_phidata *philo, char **argv, int i);
int					mutex_init(t_phidata *philo);
int					check_arg(char **argv);

int					ft_atoi(const char *nptr);
void				ft_putstr_fd(char *str, int fd);
int					ft_strlen(const char *str);
int					handle_error(char *str, t_phidata *philo, int err);
void				*ft_calloc(size_t nmemb, size_t size);
int					mutex_destroyer(t_phidata *philo, int i);
void				free_philos(t_phidata *philo, int nbphilo);
long				get_duration(struct timeval start, struct timeval end);
long				get_time_ms(struct timeval time);
void				free_datas(t_phidata *philo, int nbphilo);
void				*routine(void *arg);
