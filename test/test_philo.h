#ifndef TEST_PHILO_H
# define TEST_PHILO_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

typedef unsigned char	t_bool;
# define TRUE 1
# define FALSE 0

# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"

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
	int				mission_done;
	int				death;
	char			*txt;
	t_philo			*list;
	int				minmeals;
	pthread_mutex_t	mmutex;
	char			*msg;
	pthread_mutex_t	pmutex;
}					t_data;

#endif
