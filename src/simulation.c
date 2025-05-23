#include "../inc/philosophers.h"

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
}

//Lancement de chaque thread: le philo du thread obtient l'heure de son dernier repas puis demarre son cycle.
void	*launch(void *data)
{
	t_data	*data_;
	int		action;

	data_ = data;
	data_->list->last_meal = get_time();
	//printf("last meal : %ld\n", data_->list->last_meal);
	//a_think(data_);
	/*if (!data_->list->ID % 2)
		a_sleep(data_);
	else
		a_think(data_);*/
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
//Verification qu'aucun philo n'est mort
void	*monitor(void *data)
{
	//int		i;
	t_data	*data_;
	//t_bool	dead;

	data_ = data;
	while (1)
	{
		if (data_->dead)
			return (data);
		/*i = 0;
		while (i < data_->nphilo)
		{
			//if (is_dead(data_->list->dead))
			//	return (data);
			dead = data_->list->dead;
			printf("%d dead : %s\n", data_->list->ID, dead? "true":"false");
			data_->list = data_->list->next;
			i ++;
		}*/
	}
	return (data);
}

//Creation des threads (le surveillant + autant que de philos)
void	simulation(t_data *data)
{
	pthread_t	**thread;
	pthread_t	*monitoring;
	int			i;

	printf("%s===========================\n--- START OF SIMULATION ---\n===========================\n", KNRM);
	
	monitoring = malloc(sizeof(pthread_t *));
	pthread_create(monitoring, NULL, monitor, data);
	//pthread_join(*monitoring, NULL);	NOPE
	thread = (pthread_t **)malloc((data->nphilo + 1) * sizeof(pthread_t *));
	if (!thread)
		return ;
	i = 0;
	while (i < data->nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return;
		if (pthread_create(thread[i], NULL, launch, data))
			return ;
		printf("%s ID : %d\n", KWHT, data->list->ID);
		data->list = data->list->next;
		i ++;
	}
	i = 0;
	while (i < data->nphilo)
	{
		pthread_join(*thread[i], NULL);
		i ++;
	}
	printf("%s===========================\n---- END OF SIMULATION ----\n===========================\n", KNRM);
}

//pthread_create = cree un thread en lancant la fonction passee en argument, retourne un ID. Arguments: un pointeur sur un phtread_t (prendra la valeur de l'ID?), des parametres(?) generalement set a NULL, une fonction qui prend et retourne strictement un pointeur sur void, et l'argument en question?? qu'on peut set a NULL. 
//pthread_detach
//pthread_join = le process qui l'appelle attend que le thread passe en argument ait termine avant de poursuivre son propre flow.
//pthread_mutex_init
//pthread_mutex_destroy
//pthread_mutex_lock
//pthread_mutex_unlock