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

typedef struct s_output
{
	char			*msg;
	pthread_mutex_t	printm;
}					t_output;

typedef	struct s_fork
{
	t_bool			fork_free;
	pthread_mutex_t	forkm;
}					t_fork;

typedef	struct s_end
{
	t_bool			has_ended;
	pthread_mutex_t	endm;
}					t_end;

typedef struct s_philo
{
    pthread_t       thread;
	int				ID;
	time_t			last_meal;
	int				nmeal;
	struct s_fork	*fork;
	struct s_philo	*next;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				nphilo;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				meals_min;
	t_end			*ended;
	t_output		*output;
	t_philo			*list;
}					t_data;

//output
void    write_output(t_philo *philo, char *color, char *action, int type);

//get time
size_t	get_time(void);

//routine
void	*routine(void *philo);

//simulation
void	start_simulation(t_data *data);
void	end_simulation(t_philo *list);
int	    has_ended(t_data *data);

//init
int 	init_data(t_data *data);
int	    ft_atoi(const char *str);
int 	init_philo(t_data *data, t_philo *current);
t_philo	*create_list(t_data *data);
int	    init_mutex(t_philo *list);

#endif