/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/24 15:20:33 by cyglardo         ###   ########.fr       */
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
// tant qu'une condition de fin n'est pas atteinte,
// il vérifie s'il est lui-même mort, si c'est non, il essaye de manger. 
void	*routine(void *philo)
{
	t_philo	*philo_;

	philo_ = (t_philo *)philo;
	if (philo_->data->nphilo == 1)
	{
		usleep(philo_->data->time_to_die * 1000);
		death(philo_);
	}
	philo_->last_meal = get_time();
	if (philo_->id % 2 == 0)
		go_sleep(philo_);
	while (!has_ended(philo_->data))
	{
		if (get_time() - philo_->last_meal >= philo_->data->time_to_die)
		{
			death(philo_);
			return (philo_);
		}
		try_eating(philo_);
	}
	return (philo_);
}


void	*monitoring(void *monitor)
{
	t_monitor *monitor_;

	monitor_ = (t_monitor *)monitor;
	
	return (monitor_);
}
//Lancement de la simulation: 
//allocation mémoire puis création de chaque thread;
//on avance dans les deux listes tout en les liant ensemble
int	start_simulation(t_data *data, pthread_t **thread, t_philo *list)
{
	t_monitor	*monitor;
	pthread_t	*mthread;

	int	i;
	print_banner('S');
	mthread = malloc(sizeof(pthread_t *));
	if (!mthread)
		return (1);
	if (pthread_create(mthread, NULL, monitoring, monitor))
		return (1);
	i = 0;
	while (i < data->nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return (1);
		if (pthread_create(thread[i], NULL, routine, list))
			return (1);
		list = list->next;
		list->data = data;
		data->list = data->list->next;
		i ++;
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
		current = current->next;
		i ++;
	}
	pthread_mutex_destroy(&(data)->mmutex);
	pthread_mutex_destroy(&(data)->pmutex);
	pthread_mutex_destroy(&(data)->dmutex);
	print_banner('E');
	free_all(data, thread);
}
