#include "../inc/philosophers.h"

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
