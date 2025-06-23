#include "test_philo.h"

time_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
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
		write(2, "failed mutex init\n", 18);
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

int	has_ended(t_data *data)
{
	t_philo	*current;
	int		i;

	i = 0;
	current = data->list;
	if (data->death)
		return (1);
	else
	{
		if (data->target->minmeal > -1)
		{
			while (i < data->nphilo)
			{
				if (current->nmeal < data->target->minmeal)
					return (0);
				current = current->next;
				i ++;
			}
			return (1);
		}
		return (0);
	}
}

void	print_output(t_philo *philo, char *color, char *msg, int type)
{
	pthread_mutex_lock(&(philo->data->output)->pmutex);
	philo->data->output->msg = msg;
	if (type == 1)
		printf("%s%s\n", color, philo->data->output->msg);
	else
		printf("%s%ld %d %s", color, get_time(),
			philo->id, philo->data->output->msg);
	pthread_mutex_unlock(&(philo->data->output)->pmutex);
}

void	a_sleep(t_philo *philo)
{
	if (!has_ended(philo->data))
	{
		print_output(philo, KMAG, "is sleeping\n", 0);
		usleep(philo->data->time_to_sleep * 1000);
		if (!has_ended(philo->data))
			print_output(philo, KCYN, "is thinking\n", 0);
	}
	else
		return ;
}

void	*routine(void *philo)
{
	t_philo	*philo_;

	philo_ = (t_philo *)philo;
	philo_->last_meal = get_time();
	if (philo_->id % 2 == 0)
		a_sleep(philo_);
	while (!has_ended(philo_->data))
	{
		if (get_time() - philo_->last_meal >= philo_->data->time_to_die)
		{
			philo_->data->death = 1;
			print_output(philo_, KRED, "died\n", 0);
			return (philo_);
		}
		if (philo_->fork_free == TRUE && philo_->next->fork_free == TRUE)
		{
			if (pthread_mutex_lock(&(philo_->fmutex)))
			{
				usleep(1000);
			}	
			philo_->fork_free = FALSE;
			print_output(philo_, KGRN, "has locked own fork\n", 0);
			if (pthread_mutex_lock(&(philo_->next->fmutex)))
			{
				pthread_mutex_unlock(&(philo_->fmutex));
				philo_->fork_free = TRUE;
				usleep(1000);
			}
			else
			{
				philo_->next->fork_free = FALSE;
				print_output(philo_, KGRN, "has locked next fork\n", 0);
				print_output(philo_, KYEL, "is eating\n", 0);
				philo_->last_meal = get_time();
				usleep(philo_->data->time_to_eat * 1000);
				philo_->nmeal ++;
				pthread_mutex_unlock(&(philo_->next->fmutex));
				//print_output(philo_, KNRM, "has unlocked next fork\n", 0);
				philo_->next->fork_free = TRUE;
				pthread_mutex_unlock(&(philo_->fmutex));
				//print_output(philo_, KNRM, "has unlocked own fork\n", 0);
				philo_->fork_free = TRUE;
				a_sleep(philo_);
			}
		}
		else
			usleep(1000);
	}
	return (philo_);
}

int	start_simulation(t_data *data, t_philo *list, pthread_t **thread)
{
	int	i;

	write(1, "-A-\n", 4);
	printf("%s\n========================\n- START OF SIMULATION -\n========================\n", KNRM);
	i = 0;
	while (i < data->nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return (1);
		if (pthread_create(thread[i], NULL, routine, list))
			return (1);
		write(1, "-B-\n", 4);
		list = list->next;
		list->data = data;
		data->list = data->list->next;
		i ++;
	}
	return (0);
}

void	end_simulation(t_data *data, pthread_t **thread)
{
	int		i;
	t_philo	*current;

	i = 0;
	current = data->list;
	while (i < data->nphilo)
	{	
		printf("-C-\n");
		pthread_join(*thread[i], NULL);
		pthread_mutex_destroy(&(current->fmutex));
		current = current->next;
		i ++;
	}
	pthread_mutex_destroy(&(data->output)->pmutex);
	pthread_mutex_destroy(&(data->target)->mutex);
	printf("%s\n==========================\n--- END OF SIMULATION ---\n==========================\n", KNRM);
}

int	init_mutex(t_data *data)
{
	t_output	output;

	data->output = &output;
	if (pthread_mutex_init(&(data->output)->pmutex, NULL) == -1)
	{
		write(2, "failed mutex init\n", 18);
		return (1);
	}
	if (pthread_mutex_init(&(data->target)->mutex, NULL) == -1)
	{
		write(2, "failed mutex init\n", 18);
		return (1);
	}
	return (0);
}

int	init_data(int argc, char **argv, t_data *data)
{	
	t_target	target;

	data->target = &target;
	data->nphilo = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	data->mission_done = 0;
	data->death = 0;
	if (argc == 6)
		data->target->minmeal = atoi(argv[5]);
	else
		data->target->minmeal = -1;
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo		*list;
	t_data		data;
	pthread_t	**thread;

	if (argc < 5 || argc > 6)
		return (1);
	if (init_data(argc, argv, &data))
		return (1);
	list = create_list(data.nphilo);
	if (!list)
		return (1);
	data.list = list;
	list->data = &data;
	if (init_mutex(&data))
		return (1);
	thread = (pthread_t **)malloc((data.nphilo + 1) * sizeof(pthread_t *));
	if (!thread)
		return (1);
	if (start_simulation(&data, list, thread))
		return (1);
	end_simulation(&data, thread);
	return (0);
}
