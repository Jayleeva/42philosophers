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
	int				ID;
	char			action;
	t_bool			fork_free;
	size_t			last_meal;
	int				nmeal;
	t_bool			dead;
	struct s_philo	*next;
}					t_philo;

typedef struct s_data
{
	int		nphilo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		meals_min;
	t_bool	dead;
	t_philo	*list;
}			t_data;

//launch
void	simulation(t_data *data);
void	*launch(void *data);

//actions
t_bool	is_dead(t_data *data);
int		a_think(t_data *data);
int		a_eat(t_data *data);
int		a_sleep(t_data *data);

//utils
size_t	get_time(void);
int		ft_atoi(const char *str);
void	free_list(t_philo *head);

#endif