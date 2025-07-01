/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/07/01 14:34:15 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//Le philo dort pendant le time_to_sleep puis réfléchit
void	go_sleep(t_philo *philo)
{
	if (must_stop(philo))
		return ;
	print_output(philo, KMAG, "is sleeping\n");
	usleep(philo->data->time_to_sleep * 1000);
	if (must_stop(philo))
		return ;
	print_output(philo, KCYN, "is thinking\n");
}

//Le philo mange pendant time_to_eat, 
//incremente son nombre de repas, 
//actualise le temps de son dernier repas
void	eat(t_philo *philo)
{
	time_t	time_to_eat;

	print_output(philo, KYEL, "is eating\n");
	if (!pthread_mutex_lock(&(philo->data->tte_mtx)))
	{
		time_to_eat = philo->data->time_to_eat;
		pthread_mutex_unlock(&(philo->data->tte_mtx));
		usleep(time_to_eat * 1000);
	}
	if (!pthread_mutex_lock(&(philo->next->lmeal_mtx)))
	{
		philo->last_meal = get_time(philo->data);
		philo->nmeal ++;
		pthread_mutex_unlock(&(philo->next->lmeal_mtx));
	}
	pthread_mutex_unlock(&(philo->next->f_mtx));
	pthread_mutex_unlock(&(philo->f_mtx));
}

//Le philo essaie de lock ses fourchettes
// s'il réussit, il mange puis va dormir,
int	try_eating(t_philo *philo)
{
	if (!pthread_mutex_lock(&(philo->f_mtx)))
	{
		if (must_stop(philo))
			return (1);
		print_output(philo, KGRN, "has taken a fork\n");
		if (!pthread_mutex_lock(&(philo->next->f_mtx)))
		{
			if (must_stop(philo))
				return (2);
			print_output(philo, KGRN, "has taken a fork\n");
			eat(philo);
			go_sleep(philo);
		}
	}
	return (0);
}

//La variable death de la structure principale est lock pour être mise à 1
void	death(t_philo *philo)
{
	if (!pthread_mutex_lock(&(philo->data->stop_mtx)))
	{
		philo->data->stop = 1;
		pthread_mutex_unlock(&(philo->data->stop_mtx));
		print_output(philo, KRED, "died\n");
	}
}
