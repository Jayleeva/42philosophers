/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/30 14:29:17 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//L'unique philosophe attend jusqu'a sa mort 
void	one_philo(t_philo *philo)
{
	usleep(philo->data->time_to_die * 1000);
	death(philo);
}

//Join, destroy et free si un seul philosophe
void	end_one_philo(t_data *data, pthread_t **thread)
{
	pthread_join(*thread[0], NULL);
	pthread_mutex_destroy(&(data->list->fmutex));
	pthread_mutex_destroy(&(data->list->pmmutex));
	pthread_mutex_destroy(&(data)->mmutex);
	pthread_mutex_destroy(&(data)->pmutex);
	pthread_mutex_destroy(&(data)->smutex);
	print_banner('E');
	free(data->list);
	free(thread[0]);
	free(thread);
}
