/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/07/01 17:10:40 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//Parsing / initialisation des variables de la structure principale
int	init_data(int argc, char **argv, t_data *data)
{
	data->nphilo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->stop = 0;
	if (argc == 6)
		data->minmeals = ft_atoi(argv[5]);
	else
		data->minmeals = -1;
	return (0);
}

//Initialisation des mutex autres que les fourchettes
int	init_mutex(t_data *data)
{
	if (pthread_mutex_init(&(data)->print_mtx, NULL) == -1
		|| pthread_mutex_init(&(data)->stop_mtx, NULL) == -1
		|| pthread_mutex_init(&(data)->time_mtx, NULL) == -1)
	{
		write(2, "failed mutex init\n", 18);
		return (1);
	}
	return (0);
}

//Initiatlisation des variables du philo et du mutex de sa fourchette
void	init_philo(t_philo *current)
{
	static int	i = 1;

	current->id = i;
	current->fork_free = TRUE;
	current->last_meal = -1;
	current->nmeal = 0;
	current->next = NULL;
	if (pthread_mutex_init(&(current->f_mtx), NULL) == -1
		|| pthread_mutex_init(&(current->lmeal_mtx), NULL) == -1)
	{
		write(2, "failed mutex init\n", 18);
		return ;
	}
	i ++;
}

//Création de la liste chaînée listant les philos
t_philo	*create_list(int nphilo)
{
	t_philo	*head;
	t_philo	*current;

	if (nphilo == 0)
		return (NULL);
	head = (t_philo *)malloc(sizeof(t_philo));
	if (!head)
		return (NULL);
	init_philo(head);
	current = head;
	if (nphilo == 1)
	{
		current->next = head;
		return (head);
	}
	return (create_list_utils(head, current, nphilo));
}

//Suite de la création de la liste chaînée listant les philos
t_philo	*create_list_utils(t_philo *head, t_philo *current, int nphilo)
{
	int	i;

	i = 1;
	while (i < nphilo)
	{
		while (current->next)
			current = current->next;
		current->next = (t_philo *)malloc(sizeof(t_philo));
		if (!current->next)
			return (NULL);
		init_philo(current->next);
		i ++;
	}
	current->next->next = head;
	return (head);
}
