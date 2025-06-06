#include "../inc/philosophers.h"

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
}

t_bool	has_simulation_ended(t_data *data)
{
	if (data->dead || is_dead(data) || !data->nphilo)
	{
		//printf("AAAAAAAAAAAAAAAAAAAAAAH\n");
		return (TRUE);
	}
	return (FALSE);
}

//Lancement de chaque thread "philo": le philo concerné obtient l'heure de son dernier repas puis demarre son cycle. Si son ID est pair, il commence par penser/manger, puis il dort; sinon, il commence par dormir, puis il pense/mange; il recommence jusqu'à la fin de la simulation. S'il meurt, on sort de la boucle et on renvoie *data.
void	*routine(void *data)
{
	t_data	*data_;

	data_ = data;
	data_->list->last_meal = get_time();
	if (data_->list->ID % 2 == 0)
	{
		while (!has_simulation_ended(data_))
		{
			if (a_think(data_))
				break ;
			if (a_sleep(data_))
				break;
		}
	}
	else
	{
		while (!has_simulation_ended(data_))
		{
			if (a_sleep(data_))
				break ;
			if (a_think(data_))
				break;
		}
	}
	return (data);
}

//Verification qu'aucun philo n'est mort
void	*monitor(void *data)
{
	t_data	*data_;

	data_ = data;
	while (1)
	{
		if (data_->dead)
			return (data);
	}
	return (data);
}

void	end_simulation(t_data *data, pthread_t **thread, int nphilo)
{
	int	i;
	
	i = 0;
	while (i < nphilo)
	{
		pthread_join(*thread[i], NULL);
		i ++;
	}
	write_output(&data->mutex, KNRM, -1, END);
	pthread_mutex_destroy(&(data->mutex).deadm);
	pthread_mutex_destroy(&(data->mutex).printm);
}

//Creation des threads (le surveillant + autant que de philos)
void	start_simulation(t_data *data)
{
	pthread_t	**thread;
	//pthread_t	*monitoring;
	int			i;
	int			nphilo;

	nphilo = data->nphilo;
	write_output(&data->mutex, KNRM, -1, START);
	/*monitoring = malloc(sizeof(pthread_t *));
	pthread_create(monitoring, NULL, monitor, data);*/
	thread = (pthread_t **)malloc((data->nphilo + 1) * sizeof(pthread_t *));
	if (!thread)
		return ;
	i = 0;
	while (i < data->nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return;
		if (pthread_create(thread[i], NULL, routine, data))
			return ;
		data->list = data->list->next;
		i ++;
	}
	if (has_simulation_ended(data))
	{
		end_simulation(data, thread, nphilo);
	}
}

//pthread_create = cree un thread en lancant la fonction passee en argument, retourne un ID. Arguments: un pointeur sur un phtread_t (prendra la valeur de l'ID?), des parametres(?) generalement set a NULL, une fonction qui prend et retourne strictement un pointeur sur void, et l'argument en question?? qu'on peut set a NULL. 
//pthread_detach
//pthread_join = le process qui l'appelle attend que le thread passe en argument ait termine avant de poursuivre son propre flow. Ne pas le mettre juste après le create parce que fait attendre le main jusqu'à ce que le thread soit fini.
//pthread_mutex_init
//pthread_mutex_destroy
//pthread_mutex_lock
//pthread_mutex_unlock