/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/07/01 17:14:01 by cyglardo         ###   ########.fr       */
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
	pthread_mutex_t	f_mtx;
	time_t			last_meal;
	pthread_mutex_t	lmeal_mtx;
	int				nmeal;
	struct s_philo	*next;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	time_t				time;
	pthread_mutex_t		time_mtx;
	int					nphilo;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	int					minmeals;
	int					stop;
	pthread_mutex_t		stop_mtx;
	pthread_mutex_t		print_mtx;
	t_philo				*list;
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
void	end_one_philo(t_data *data);

//simulation
void	*routine(void *philo);
int		start_simulation(t_data *data, pthread_t **thread, t_philo *list);
void	join(t_data *data, pthread_t **thread);
void	end_simulation(t_data *data, pthread_t **thread);

//monitoring
int		has_started(t_data *data);
int		start_monitor(t_data *data, pthread_t **thread, int i);
void	*monitoring(void *monitor);

//actions
void	go_sleep(t_philo *philo);
int		try_eating(t_philo *philo_);
void	eat(t_philo *philo);
void	death(t_philo *philo);

//ending conditions
int		has_ended(t_data *data);
int		has_someone_died(t_data *data);
int		is_minmeals_done(t_data *data);
void	free_all(t_data *data, pthread_t **thread);

//utils
int		must_stop(t_philo *philo);
time_t	get_init_time(void);
time_t	get_time(t_data *data);
void	print_output(t_philo *philo, char *color, char *msg);

//decoration
void	print_banner(char c);

#endif