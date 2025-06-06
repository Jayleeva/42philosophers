#include "philosophers.h"
#include "ft_atoi.c"

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
}
int	init_philo(t_data *data, t_philo *current)
{
	static int	i = 1;
	t_fork		fork_;

	//current->thread = 0;
	current->ID = i;
	current->last_meal = 0;
	current->nmeal = 0;
	current->fork = &fork_;
	if (pthread_mutex_init(&(current->fork)->forkm, NULL))
		return (1);
	current->fork->fork_free = TRUE;
	current->next = NULL;
	current->data = data;
	i ++;
	return (0);
}

t_philo	*create_list(t_data *data)
{
	t_philo	*head;
	t_philo	*current;
	int		i;

	head = (t_philo *)malloc(sizeof(t_philo));
	if (!head)
		return (NULL);
	if (init_philo(data, head))
		return (NULL); // FREE!!
	current = head;
	i = 1;
	while (i < data->nphilo)
	{
		while (current->next)
			current = current->next;
		current->next = (t_philo *)malloc(sizeof(t_philo));
		if (!current->next)
			return (NULL); // FREE!!
		if(init_philo(data, current->next))
			return (NULL); // FREE!!
		i ++;
	}
	current->next->next = head;
	return (head);
}
//checks every philosopher to see if they've eaten enough or if one of them is dead
int	has_ended(t_data *data)
{
	t_philo	*current;
	int		i;
	int		j;

	current = data->list;
	i = 0;
	j = 0;
	while (i < data->nphilo)
	{
		if (get_time() - current->last_meal >= (size_t)data->time_to_die)
		{
			printf("ONE DEAD\n");
			return (1);
		}
		if (data->meals_min > 0 && current->nmeal >= data->meals_min)
			j ++;
		current = current->next;
		i++;
	}
	if (j == i)
	{
		printf("ALL HAVE EATEN ENOUGH\n");
		return (1);
	}
	return (0);
}
//locks the print mutex to print the received output, then unlocks it
void    write_output(t_philo *philo, char *color, char *action, int type)
{
    pthread_mutex_lock(&(philo->data->output)->printm);
    if (type == 0)
        printf("%s%s\n", KNRM, action);
    else
        printf("%s%ld %d %s\n", color, get_time(), philo->ID, action);
    pthread_mutex_unlock(&(philo->data->output)->printm);
}

void	*routine(void *philo)
{
	t_philo	*philo_;

	philo_ = (t_philo *)philo;
	while (!has_ended(philo_->data))
	{
		pthread_mutex_lock(&(philo_->fork)->forkm);
		if (philo_->fork->fork_free)
		{
			philo_->fork->fork_free = FALSE;
			pthread_mutex_lock(&(philo_->next->fork)->forkm);
			if (philo_->next->fork->fork_free)
			{
				philo_->next->fork->fork_free = FALSE;
				write_output(philo_, KGRN, EAT, 0);
				usleep(philo_->data->time_to_eat);
				philo_->next->fork->fork_free = TRUE;
			}
			pthread_mutex_unlock(&(philo_->next->fork)->forkm);
			philo_->fork->fork_free = TRUE;
		}
		pthread_mutex_unlock(&(philo_->fork)->forkm);
		philo_->nmeal ++;
		philo_->last_meal = get_time();
	}
	return (philo_);
}
//creates all the threads, starting their routines
void	start_simulation(t_data *data)
{
	pthread_t	**thread;
	int			i;

	write_output(data->list, KNRM, START, 0);
	thread = (pthread_t **)malloc((data->nphilo + 1) * sizeof(pthread_t *));
	if (!thread)
		return ;
	i = 0;
	while (i < data->nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return;
		if (pthread_create(thread[i], NULL, routine, data->list))
			return ;
		data->list = data->list->next;
		i ++;
	}
}
//joins all the threads and destroys the mutexes
void	end_simulation(t_philo *list)
{
	int	i;

	i = list->data->nphilo;
	while (i > 0)
	{
		pthread_join(list->thread, NULL);
		list = list->next;
		i--;
	}
	write_output(list, KNRM, END, 0);
	pthread_mutex_destroy(&(list->data->ended)->endm);
	pthread_mutex_destroy(&(list->data->output)->printm);
}

int	init_mutex(t_philo *list)
{
	/*t_philo	*current;
	int		i;
	
	current = list;
	i = 0;
	while (i < list->data->nphilo)
	{
		
		if (pthread_mutex_init(&(current->fork)->forkm, NULL))
			return (1);
		current = current->next;
		i ++;
	}*/
	write(1, "-A-\n", 4);
	if (pthread_mutex_init(&(list->data->ended)->endm, NULL))
		return (1);
	write(1, "-B-\n", 4);
	if (pthread_mutex_init(&(list->data->output)->printm, NULL))
		return (1);
	write(1, "-C-\n", 4);
	return (0);
}
int	init_data(t_data *data)
{
	t_end		ended;
	t_output	output;

	data->nphilo = 0;
	data->time_to_die = 0;
	data->time_to_eat = 0;
	data->time_to_sleep = 0;
	data->meals_min = -1;

	data->ended = &ended;
	data->ended->has_ended = FALSE;
	data->output = &output;
	if (pthread_mutex_init(&(data->ended)->endm, NULL))
		return (1);
	if (pthread_mutex_init(&(data->output)->printm, NULL))
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo *list;

	if (init_data(&data)) // BUG SUITE A CA: pthread_tpp change priority, assertion failed. OU boucle infinie si un write
		return (1);
	//write(1, "-A-\n", 4);
	data.nphilo = ft_atoi(argv[1]);
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data.meals_min = ft_atoi(argv[5]);
	if (data.nphilo <= 1)
		return (1);
	list = create_list(&data);
	if (!list)
		return (1);
	data.list = list;
	list->data = &data;
	start_simulation(&data);
	end_simulation(list);
}
