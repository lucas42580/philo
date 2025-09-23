/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaysant <lpaysant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:41:58 by lpaysant          #+#    #+#             */
/*   Updated: 2025/09/23 13:49:00 by lpaysant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum e_status
{
	GO,
	STOP
}							t_status;
typedef struct s_data		t_data;
typedef struct s_phidata	t_phidata;

struct						s_data
{
	long					start_time;
	int						nbphilo;
	int						ttdie;
	int						tteat;
	int						ttsleep;
	int						maxmeal;
	int						state;
	pthread_t				monitor;
	pthread_mutex_t			start_mutex;
	pthread_mutex_t			state_mutex;
	pthread_mutex_t			print_mutex;
};
struct						s_phidata
{
	pthread_t				thread;
	int						id;
	int						nbmeal;
	long					lastmeal;
	bool					leftfork;
	bool					*rightfork;
	bool					one_philo;
	pthread_mutex_t			meal_mutex;
	pthread_mutex_t			leftfork_mutex;
	pthread_mutex_t			*rightfork_mutex;
	t_data					*data;
};

t_data						*get_data(char **argv);
int							init_loop(t_phidata *philo, t_data *data);
int							data_mutex_init(t_data *data);
int							mutex_init(t_phidata *philo, int i);
int							check_arg(char **argv);

int							ft_atoi(const char *nptr);
void						ft_putstr_fd(char *str, int fd);
int							ft_strlen(const char *str);
void						*ft_calloc(size_t nmemb, size_t size);

int							handle_error(char *str, t_phidata *philo, int err);
int							mutex_destroyer(t_phidata *philo, int i);
void						free_philos(t_phidata *philo, int nbphilo);
void						free_datas(t_phidata *philo, int nbphilo);

long						get_duration(struct timeval start,
								struct timeval end);
long						get_time_ms(struct timeval time);

void						*routine(void *arg);
bool						death_check(t_phidata *philo);
void						threads_checking(t_phidata *philo);
int							print_lock(t_phidata *philo, char *str);
int							ft_wait(t_phidata *philo, long time);
