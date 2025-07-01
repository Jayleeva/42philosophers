/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/07/01 14:47:21 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	end(t_data *data, pthread_t **thread)
{
	if (data->nphilo == 1)
		end_one_philo(data);
	else
	{
		while (1)
		{
			if (!pthread_mutex_lock(&(data->stop_mtx)))
			{
				if (data->stop)
				{
					pthread_mutex_unlock(&(data->stop_mtx));
					break;
				}
				pthread_mutex_unlock(&(data->stop_mtx));
			}
		}
		end_simulation(data, thread);
	}
}

//Lancement et arrêt du programme : 
// parsing, initialisation var et mutex, lancement et arrêt simulation
int	main(int argc, char **argv)
{
	t_philo		*list;
	t_data		data;
	pthread_t	**thread;

	if (argc < 5 || argc > 6)
		return (1);
	init_data(argc, argv, &data);
	list = create_list(data.nphilo);
	if (!list)
		return (1);
	data.list = list;
	list->data = &data;
	init_mutex(&data);
	thread = (pthread_t **)malloc((data.nphilo + 2) * sizeof(pthread_t *));
	if (!thread)
		return (1);
	if (start_simulation(&data, thread, list))
		return (1);
	end(&data, thread);
	return (0);
}
