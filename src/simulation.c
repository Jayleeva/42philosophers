#include "../inc/philosophers.h"

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
}

//Lancement de chaque thread: le philo du thread obient l'heure de son dernier repas puis demarre son cycle.
void	*launch(void *data)
{
	t_data	*data_;

	data_ = data;
	data_->list->last_meal = get_time();
	printf("last meal : %ld\n", data_->list->last_meal);
	//printf("ID: %ld\n", );
	/*while (data_->nphilo) // pour compter les repas; pour que marche aussi dès la première mort, faire en sorte que la première mort élimine tous les autres. mettre aussi une mutex sur le nombre de philo
	{
		if (a_think(data_))
		{
			data_->nphilo = 0; // fermer tous les threads
			break ;
		}
	}*/
	return (data);
}

//Creation des threads (autant que de philos)
void	simulation(t_data *data)
{
	pthread_t	**thread;
	int			i;
	t_philo		*current;

	printf("%s===========================\n--- START OF SIMULATION ---\n===========================\n", KNRM);
	//pthread_create(&thread, NULL, launch(data), NULL);
	thread = malloc(sizeof(pthread_t **));
	if (!thread)
		return ;
	current = data->list;
	i = 0;
	while (i < data->nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return;
		current->ID = pthread_create(thread[i], NULL, launch(data), NULL);
		printf("ID : %d\n", current->ID);
		current = current->next;
		//pthread_create(thread[i], NULL, launch, (void *)&thread);
		i ++;
	}
	printf("%s===========================\n---- END OF SIMULATION ----\n===========================\n", KNRM);
}

//pthread_create = cree un thread en lancant la fonction passee en argument, retourne un ID. Arguments: un pointeur sur un phtread_t (prendra la valeur de l'ID?), des parametres(?) generalement set a NULL, une fonction qui prend et retourne strictement un pointeur sur void, et l'argument en question?? qu'on peut set a NULL. 
//pthread_detach
//pthread_join
//pthread_mutex_init
//pthread_mutex_destroy
//pthread_mutex_lock
//pthread_mutex_unlock