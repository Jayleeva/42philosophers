/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_conditions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/26 16:17:58 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//Vérification : est-ce qu'un des philo est déjà mort?
int	has_someone_died(t_data *data)
{
	int		i;
	t_philo	*current;

	current = data->list;
	i = 0;
	while (i < data->nphilo)
	{
		if (get_time() - current->last_meal >= current->data->time_to_die)
		{
			death(current);
			return (1);
		}
		current = current->next;
		i ++;
	}
	return (0);
}

//Vérification : est-ce que chaque philo a eu le minimum de repas demandé?
int	is_minmeals_done(t_data *data)
{
	int		i;
	t_philo	*current;

	current = data->list;
	if (!pthread_mutex_lock(&(data->mmutex)))
	{
		if (data->minmeals > -1)
		{
			i = 0;
			while (i < data->nphilo)
			{
				if (current->nmeal < data->minmeals)
				{
					pthread_mutex_unlock(&(data->mmutex));
					return (0);
				}
				current = current->next;
				i ++;
			}
			pthread_mutex_unlock(&(data->mmutex));
			return (1);
		}
		pthread_mutex_unlock(&(data->mmutex));
	}
	return (0);
}

//Vérification : une condition de fin a-t-elle été atteinte?
int	has_ended(t_data *data)
{
	if (has_someone_died(data))
		return (1);
	else if (is_minmeals_done(data))
		return (1);
	else
		return (0);
}
