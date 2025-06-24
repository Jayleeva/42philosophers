/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/24 15:58:01 by cyglardo         ###   ########.fr       */
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
	struct s_philo	*next;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				nphilo;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				death;
	t_philo			*list;
	int				minmeals;
	pthread_mutex_t	mmutex;
	char			*msg;
	pthread_mutex_t	pmutex;
}					t_data;

//init
int		ft_atoi(const char *str);

//simulation
int		has_ended(t_data *data);
void	a_sleep(t_philo *philo);
void	try_eating(t_philo *philo_);
void	*routine(void *philo);
int		start_simulation(t_data *data, pthread_t **thread, t_philo *list);
void	end_simulation(t_data *data, pthread_t **thread);

//utils
time_t	get_time(void);
void	print_output(t_philo *philo, char *color, char *msg, int type);
void	free_all(t_data *data, pthread_t **thread);

#endif