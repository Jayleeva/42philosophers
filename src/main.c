#include "../inc/philosophers.h"

void	free_all(t_data *data, pthread_t **thread)
{
	int		i;
	t_philo	*current;

	free(thread);
	i = 0;
	current = data->list;
	while (i < data->nphilo)
	{
		free(current);
		current = current->next;
		i ++;
	}
}

void	write_output(t_philo *philo, char *color, char *msg, int type)
{
	pthread_mutex_lock(&(philo->data)->pmutex);
	philo->data->msg = msg;
	if (type == 1)
		printf("%s%s\n", color, philo->data->msg);
	else
		printf("%s%ld %d %s", color, get_time(), philo->id, philo->data->msg);
	pthread_mutex_unlock(&(philo->data)->pmutex);
}

time_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
}

int	init_data(int argc, char **argv, t_data *data)
{
	data->nphilo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->death = 0;
	if (argc == 6)
		data->minmeals = ft_atoi(argv[5]);
	else
		data->minmeals = -1;
	return (0);
}

int	init_mutex(t_data *data)
{
	if (pthread_mutex_init(&(data)->pmutex, NULL) == -1)
	{
		write(2, "failed mutex init\n", 18);
		return (1);
	}
	if (pthread_mutex_init(&(data)->mmutex, NULL) == -1)
	{
		write(2, "failed mutex init\n", 18);
		return (1);
	}
	return (0);
}

void	init_philo(t_philo *current)
{
	static int	i = 1;

	current->id = i;
	current->fork_free = TRUE;
	current->last_meal = 0;
	current->nmeal = 0;
	current->next = NULL;
	if (pthread_mutex_init(&(current->fmutex), NULL) == -1)
	{
		printf("failed mutex init\n");
		return ;
	}
	i ++;
}

t_philo	*create_list(int nphilo)
{
	t_philo	*head;
	t_philo	*current;
	int		i;

	head = (t_philo *)malloc(sizeof(t_philo));
	if (!head)
		return (NULL);
	init_philo(head);
	current = head;
	i = 1;
	while (i < nphilo)
	{
		while (current->next)
			current = current->next;
		current->next = (t_philo *)malloc(sizeof(t_philo));
		if (!current->next)
			return (NULL);
		init_philo(current->next);
		i ++;
	}
	current->next->next = head;
	return (head);
}


int	main(int argc, char **argv)
{
	t_philo		*list;
	t_data		data;
	pthread_t	**thread;

	if (argc < 5 || argc > 6)
		return (1);
	init_data(argc, argv, &data);
	list = create_list(data.nphilo);
	if (!list)
		return (1);
	data.list = list;
	list->data = &data;
	init_mutex(&data);
	thread = (pthread_t **)malloc((data.nphilo + 1) * sizeof(pthread_t *));
	if (!thread)
		return (1);
	if (start_simulation(&data, thread, list))
		return (1);
	end_simulation(&data, thread);
	return (0);
}
