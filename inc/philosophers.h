/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/30 15:13:37 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef unsigned char	t_bool;
# define TRUE 1
# define FALSE 0

# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"

typedef struct s_philo
{
	int				id;
	t_bool			fork_free;
	pthread_mutex_t	fmutex;
	int				nmeal;
	time_t			last_meal;
	pthread_mutex_t	pmmutex;
	struct s_philo	*next;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	struct timeval		stime;
	time_t				time;
	pthread_mutex_t		tmutex;
	int					nphilo;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	pthread_mutex_t		temutex;
	int					stop;
	pthread_mutex_t		smutex;
	t_philo				*list;
	int					minmeals;
	pthread_mutex_t		mmutex;
	char				*msg;
	pthread_mutex_t		pmutex;
	t_philo				*monitor;
}						t_data;

//parsing and init
int		ft_atoi(const char *str);
t_philo	*create_list(int nphilo);
t_philo	*create_list_utils(t_philo *head, t_philo *current, int nphilo);
void	init_philo(t_philo *current);
int		init_mutex(t_data *data);
int		init_data(int argc, char **argv, t_data *data);

//un seul philosophe
void	one_philo(t_philo *philo);
void	end_one_philo(t_data *data, pthread_t **thread);

//simulation
void	*routine(void *philo);
void	philo_loop(t_philo *philo);
int		start_simulation(t_data *data, pthread_t **thread, t_philo *list);
void	end_simulation(t_data *data, pthread_t **thread);

//monitoring
int		has_started(t_data *data);
int		start_monitor(t_data *data, pthread_t **thread, int i);
void	*monitoring(void *monitor);

//actions
void	go_sleep(t_philo *philo);
int		try_eating(t_philo *philo_);
void	try_eating_utils(t_philo *philo);
void	death(t_philo *philo);

//ending conditions
int		has_ended(t_data *data);
int		has_someone_died(t_data *data);
int		is_minmeals_done(t_data *data);

//utils
int		must_stop(t_philo *philo);
time_t	get_init_time(void);
time_t	get_time(t_data *data);
void	print_output(t_philo *philo, char *color, char *msg);
void	free_all(t_data *data, pthread_t **thread);

//decoration
void	print_banner(char c);

#endif