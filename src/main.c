/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/07/01 17:48:29 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	args_invalid(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j ++;
		}
		i ++;
	}
	return (0);
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
	if (args_invalid(argv))
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
	if (data.nphilo == 1)
		end_one_philo(&data);
	else
		end_simulation(&data, thread);
	return (0);
}
