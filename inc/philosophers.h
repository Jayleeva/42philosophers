#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_h
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef unsigned char	t_bool;
# define TRUE 1
# define FALSE 0

# define THINK "is thinking"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define DEAD "is dead"
# define FINISH "has finished all her meals"

# define START "===========================\n--- START OF SIMULATION ---\n==========================="
# define END "===========================\n---- END OF SIMULATION ----\n==========================="

# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"

/*typedef	struct s_fork
{
	t_bool			fork_free;
	pthread_mutex_t	forkm;
}					t_fork;*/


typedef struct s_philo
{
	int				ID;
	char			action;
	t_bool			fork_free;
	//struct s_fork	fork;
	time_t			last_meal;
	//pthread_mutex_t	last_mealm;
	int				nmeal;
	t_bool			dead;
	struct s_philo	*next;
}					t_philo;

typedef struct s_mutex
{
	pthread_mutex_t	last_mealm;
	pthread_mutex_t	forkm;
	pthread_mutex_t	deadm;
	pthread_mutex_t	printm;
}					t_mutex;

typedef struct s_data
{
	int				nphilo;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				meals_min;
	t_bool			dead;
	t_philo			*list;
	t_mutex			mutex;
}					t_data;

//launch
void	start_simulation(t_data *data);
void	*routine(void *data);

//exit
t_bool	has_simulation_ended(t_data *data);
void	end_simulation(t_data *data, pthread_t **thread, int nphilo);

//actions
t_bool	is_dead(t_data *data);
int		a_think(t_data *data);
int		a_eat(t_data *data);
int		a_sleep(t_data *data);

//output
void    write_output(t_mutex *mutex, char *color, int philo, char *action);

//utils
size_t	get_time(void);
int		ft_atoi(const char *str);
void	free_list(t_philo *head);

#endif