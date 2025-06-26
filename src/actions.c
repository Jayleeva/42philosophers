/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/26 16:23:48 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//Le philo dort pendant le time_to_sleep puis réfléchit
void	go_sleep(t_philo *philo)
{
	if (must_stop(philo))
		return ;
	print_output(philo, KMAG, "is sleeping\n", 0);
	usleep(philo->data->time_to_sleep * 1000);
	if (must_stop(philo))
		return ;
	print_output(philo, KCYN, "is thinking\n", 0);
}

//Le philo essaie de lock ses fourchettes
// s'il réussit, il mange pendant le time_to_eat puis va dormir,
// sinon, il réfléchit

//if (get_time() - tmp + philo->data->time_to_eat >= philo->data->time_to_die)
//	return ;
void	try_eating(t_philo *philo)
{
	if (!pthread_mutex_lock(&(philo->fmutex)))
	{
		print_output(philo, KGRN, "has locked own fork\n", 0);
		if (!pthread_mutex_lock(&(philo->next->fmutex)))
		{
			print_output(philo, KGRN, "has locked next fork\n", 0);
			print_output(philo, KYEL, "is eating\n", 0);
			philo->last_meal = get_time();
			usleep(philo->data->time_to_eat * 1000);
			philo->nmeal ++;
			pthread_mutex_unlock(&(philo->next->fmutex));
			pthread_mutex_unlock(&(philo->fmutex));
			go_sleep(philo);
		}
	}
	else
		print_output(philo, KCYN, "is thinking\n", 0);
}

//La variable death de la structure principale est lock pour être mise à 1
void	death(t_philo *philo)
{
	if (!pthread_mutex_lock(&(philo->data->smutex)))
	{
		philo->data->stop = 1;
		pthread_mutex_unlock(&(philo->data->smutex));
		print_output(philo, KRED, "died\n", 0);
	}
}
