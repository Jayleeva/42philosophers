/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/07/01 14:17:39 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//Verification : tous les threads ont-ils ete lances?
int	has_started(t_data *data)
{
	int		i;
	t_philo	*current;

	current = data->list;
	i = 0;
	while (i < data->nphilo)
	{
		if (!pthread_mutex_lock(&(current->lmeal_mtx)))
		{
			if (current->last_meal == -1)
			{
				pthread_mutex_unlock(&(current->lmeal_mtx));
				return (0);
			}
			pthread_mutex_unlock(&(current->lmeal_mtx));
		}
		current = current->next;
		i ++;
	}
	return (1);
}

//Routine du monitor: 
//verifie a l'infini si une condition de fin a ete atteinte
//si oui, passe le stop a TRUE avec un mutex, puis s'arrete
void	*monitoring(void *monitor)
{
	t_philo	*monitor_;

	monitor_ = (t_philo *)monitor;
	while (1)
	{
		if (has_ended(monitor_->data))
		{
			if (!pthread_mutex_lock(&(monitor_->data->stop_mtx)))
			{
				monitor_->data->stop = 1;
				pthread_mutex_unlock(&(monitor_->data->stop_mtx));
				break ;
			}
		}
	}
	return (monitor_);
}

//Initialisation du monitor,
//qui ne sera cree que lorsque tous les autres l'auront ete
int	start_monitor(t_data *data, pthread_t **thread, int i)
{
	t_philo	*monitor;

	monitor = (t_philo *)malloc(sizeof(t_philo));
	if (!monitor)
		return (1);
	init_philo(monitor);
	monitor->data = data;
	data->monitor = monitor;
	while (1)
	{
		if (has_started(data))
			break ;
	}
	thread[i] = malloc(sizeof(pthread_t *));
	if (!thread[i])
		return (free(monitor), 1);
	if (pthread_create(thread[i], NULL, monitoring, monitor))
		return (free(monitor), 1);
	return (0);
}
