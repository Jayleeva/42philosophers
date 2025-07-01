/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/07/01 14:49:16 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//L'unique philosophe attend jusqu'a sa mort 
void	one_philo(t_philo *philo)
{
	usleep(philo->data->time_to_die * 1000);
	print_output(philo, KRED, "died\n");
}

//Join, destroy et free si un seul philosophe
void	end_one_philo(t_data *data)
{
	pthread_mutex_destroy(&(data->list->f_mtx));
	pthread_mutex_destroy(&(data->list->lmeal_mtx));
	pthread_mutex_destroy(&(data)->mmeals_mtx);
	pthread_mutex_destroy(&(data)->print_mtx);
	pthread_mutex_destroy(&(data)->stop_mtx);
	print_banner('E');
	free(data->list);
}
