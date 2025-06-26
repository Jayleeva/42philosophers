/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/26 16:19:53 by cyglardo         ###   ########.fr       */
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
		if (current->last_meal == 0)
			return (0);
		current = current->next;
		i ++;
	}
	return (1);
}

void	*monitoring(void *monitor)
{
	t_philo	*monitor_;

	monitor_ = (t_philo *)monitor;
	while (1)
	{
		if (has_ended(monitor_->data))
		{
			if (!pthread_mutex_lock(&(monitor_->data->smutex)))
			{
				monitor_->data->stop = 1;
				pthread_mutex_unlock(&(monitor_->data->smutex));
				break ;
			}
		}
	}
	return (monitor_);
}

int	start_monitor(t_data *data, pthread_t **thread, int i)
{
	t_philo	*monitor;

	monitor = (t_philo *)malloc(sizeof(t_philo));
	if (!monitor)
		return (1);
	init_philo(monitor);
	monitor->data = data;
	while (1)
	{
		if (has_started(data))
			break ;
	}
	thread[i] = malloc(sizeof(pthread_t *));
	if (!thread[i])
		return (1);
	if (pthread_create(thread[i], NULL, monitoring, monitor))
		return (1);
	return (0);
}
