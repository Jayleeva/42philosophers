#include "../inc/philosophers.h"

void	init_philo(t_philo *current)
{
	static int	i = 1;

	current->ID = i;
	current->action = 'n';
	current->fork_free = TRUE;
	current->last_meal = 0;
	current->nmeal = 0;
	current->dead = FALSE;
	current->next = NULL;
	i ++;
}

//Creation de la liste chainee contenant les philos
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
			return (free_list(head), NULL);
		init_philo(current->next);
		i ++;
	}
	current->next->next = head;
	return (head);
}

void	init_data(t_data *data)
{
	data->nphilo = 0;
	data->time_to_die = 0;
	data->time_to_eat = 0;
	data->time_to_sleep = 0;
	data->meals_min = -1;
	data->dead = FALSE;
	if (pthread_mutex_init(&(data->mutex).deadm, NULL))
		return ;
	if (pthread_mutex_init(&(data->mutex).printm, NULL))
		return ;
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo *list;

	if (argc < 5)
		return (1);
	if (argc > 6)
		return (1);
	init_data(&data);
	data.nphilo = ft_atoi(argv[1]);
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data.meals_min = ft_atoi(argv[5]);
	if (data.nphilo <= 1)
		return (1);
	list = create_list(data.nphilo);
	if (!list)
		return (1);
	data.list = list;
	start_simulation(&data);
	/*if (data.dead)
		end_simulation(thread, data.nphilo);*/
	//free_list(list);
	return (0);
}