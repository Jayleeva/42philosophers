/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/30 15:14:06 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//Impression de la bannière marquant le début et la fin de la simulation
void	print_banner(char c)
{
	if (c == 'S')
	{
		printf("%s\n========================\n", KNRM);
		printf("- START OF SIMULATION -\n");
		printf("========================\n");
	}
	else if (c == 'E')
	{
		printf("%s\n==========================\n", KNRM);
		printf("--- END OF SIMULATION ---\n");
		printf("==========================\n");
	}
}

// tant qu'une condition de fin n'a pas ete atteinte,
// il essaye de manger.
void	philo_loop(t_philo *philo)
{
	int		tmp;

	while (!must_stop(philo))
	{
		tmp = try_eating(philo);
		if (tmp == 1)
			pthread_mutex_unlock(&(philo->fmutex));
		else if (tmp == 2)
		{
			pthread_mutex_unlock(&(philo->fmutex));
			pthread_mutex_unlock(&(philo->next->fmutex));
		}
	}
}

//Le thread (ou philo) prend le temps actuel comme temps du dernier repas; 
// s'il est pair, il commence par dormir;
void	*routine(void *philo)
{
	t_philo	*philo_;

	philo_ = (t_philo *)philo;
	if (philo_->data->nphilo == 1)
	{
		one_philo(philo);
		return (philo_);
	}
	if (!pthread_mutex_lock(&(philo_->pmmutex)))
	{
		philo_->last_meal = get_time(philo_->data);
		pthread_mutex_unlock(&(philo_->pmmutex));
	}
	if (philo_->id % 2 == 0)
		go_sleep(philo_);
	philo_loop(philo_);
	return (philo_);
}

//Lancement de la simulation: 
//allocation mémoire puis création de chaque thread;
//on avance dans les deux listes tout en les liant ensemble
int	start_simulation(t_data *data, pthread_t **thread, t_philo *list)
{
	int	i;

	print_banner('S');
	data->time = get_init_time();
	i = 0;
	while (i < data->nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return (1);
		list->data = data;
		if (pthread_create(thread[i], NULL, routine, list))
			return (1);
		list = list->next;
		data->list = data->list->next;
		i ++;
	}
	if (data->nphilo != 1)
	{
		if (start_monitor(data, thread, i))
			return (1);
	}
	return (0);
}

//Fin de la simulation: 
//on join les thread, détruit les mutex, libère la mémoire allouée.
void	end_simulation(t_data *data, pthread_t **thread)
{
	int		i;
	t_philo	*current;

	i = 0;
	current = data->list;
	while (i < data->nphilo)
	{
		pthread_join(*thread[i], NULL);
		pthread_mutex_destroy(&(current->fmutex));
		pthread_mutex_destroy(&(current->pmmutex));
		current = current->next;
		i ++;
	}
	pthread_join(*thread[i], NULL);
	pthread_mutex_destroy(&(data)->mmutex);
	pthread_mutex_destroy(&(data)->pmutex);
	pthread_mutex_destroy(&(data)->smutex);
	pthread_mutex_destroy(&(data)->tmutex);
	pthread_mutex_destroy(&(data)->temutex);
	print_banner('E');
	free_all(data, thread);
}
