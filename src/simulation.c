#include "../inc/philosophers.h"

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
}

void	simulation(t_data *data, t_philo *list)
{
	/*t_philo	*current;

	current = list;
	while (current)
	{
		printf("ID: %d\n", current->ID);
		current = current->next;
	}*/
	printf("%s===========================\n--- START OF SIMULATION ---\n===========================\n", KNRM);
	list->last_meal = get_time();
	while (data->nphilo) // pour compter les repas; pour que marche aussi dès la première mort, faire en sorte que la première mort élimine tous les autres. mettre aussi une mutex sur le nombre de philo
	{
		if (a_think(data, list))
		{
			data->nphilo = 0; // fermer tous les threads
			break ;
		}
	}
	printf("%s===========================\n---- END OF SIMULATION ----\n===========================\n", KNRM);
}

//pthread_create
//pthread_detach
//pthread_join
//pthread_mutex_init
//pthread_mutex_destroy
//pthread_mutex_lock
//pthread_mutex_unlock