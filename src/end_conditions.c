/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_conditions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/07/01 13:04:16 by cyglardo         ###   ########.fr       */
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
		if (!pthread_mutex_lock(&(current->lmeal_mtx)))
		{
			if (get_time(data) - current->last_meal
				>= current->time_to_die)
			{
				pthread_mutex_unlock(&(current->lmeal_mtx));
				death(current);
				return (1);
			}
			pthread_mutex_unlock(&(current->lmeal_mtx));
			current = current->next;
			i ++;
		}
	}
	return (0);
}

//Vérification : est-ce que chaque philo a eu le minimum de repas demandé?
int	is_minmeals_done(t_data *data)
{
	int		i;
	t_philo	*current;

	current = data->list;
	if (current->minmeals > -1)
	{
		i = 0;
		while (i < data->nphilo)
		{
			if (current->nmeal < data->minmeals)
			{
				return (0);
			}
			current = current->next;
			i ++;
		}
		return (1);
	}
	return (0);
}

//Vérification : une condition de fin a-t-elle été atteinte?
int	has_ended(t_data *data)
{
	if (has_someone_died(data) || is_minmeals_done(data))
		return (1);
	else
		return (0);
}

//Libération de toute la mémoire allouée
void	free_all(t_data *data, pthread_t **thread)
{
	int		i;
	t_philo	*current;
	t_philo	*temp;

	i = 0;
	current = data->list;
	while (i < data->nphilo)
	{
		temp = current;
		current = current->next;
		free(temp);
		free(thread[i]);
		i ++;
	}
	free(thread[i]);
	free(thread);
	free(data->monitor);
}
