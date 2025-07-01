/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/07/01 17:08:21 by cyglardo         ###   ########.fr       */
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

//Le thread (ou philo) prend le temps actuel comme temps du dernier repas; 
// s'il est pair, il commence par dormir;
// tant qu'une condition de fin n'a pas ete atteinte,
// il essaye de manger.
void	*routine(void *philo)
{
	t_philo	*philo_;

	philo_ = (t_philo *)philo;
	if (!pthread_mutex_lock(&(philo_->lmeal_mtx)))
	{
		philo_->last_meal = get_time(philo_->data);
		pthread_mutex_unlock(&(philo_->lmeal_mtx));
	}
	if (philo_->id % 2 == 0)
		go_sleep(philo_);
	while (!must_stop(philo))
	{
		try_eating(philo);
	}
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
	if (data->nphilo == 1)
	{
		one_philo(list);
		return (0);
	}
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
	if (start_monitor(data, thread, i))
		return (1);
	return (0);
}

void	join(t_data *data, pthread_t **thread)
{
	int		i;
	t_philo	*current;

	i = 0;
	current = data->list;
	while (i < data->nphilo)
	{
		pthread_join(*thread[i], NULL);
		current = current->next;
		i ++;
	}
}

//Fin de la simulation: 
//on join les thread, détruit les mutex, libère la mémoire allouée.
void	end_simulation(t_data *data, pthread_t **thread)
{
	int		i;
	t_philo	*current;

	join(data, thread);
	i = 0;
	current = data->list;
	while (i < data->nphilo)
	{
		pthread_mutex_destroy(&(current->f_mtx));
		pthread_mutex_destroy(&(current->lmeal_mtx));
		current = current->next;
		i ++;
	}
	pthread_join(*thread[i], NULL);
	pthread_mutex_destroy(&(data)->print_mtx);
	pthread_mutex_destroy(&(data)->stop_mtx);
	pthread_mutex_destroy(&(data)->time_mtx);
	print_banner('E');
	free_all(data, thread);
}
