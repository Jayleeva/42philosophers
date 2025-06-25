/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/24 15:20:33 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//Le philo dort pendant le time_to_sleep puis réfléchit
void	go_sleep(t_philo *philo)
{
	if (!has_ended(philo->data))
	{
		print_output(philo, KMAG, "is sleeping\n", 0);
		usleep(philo->data->time_to_sleep * 1000);
		if (!has_ended(philo->data))
			print_output(philo, KCYN, "is thinking\n", 0);
	}
	else
		return ;
}

//Le philo essaie de lock ses fourchettes
// s'il réussit, il mange pendant le time_to_eat puis va dormir,
// sinon, il réfléchit
void	try_eating(t_philo *philo_)
{
	if (!pthread_mutex_lock(&(philo_->fmutex)))
	{
		print_output(philo_, KGRN, "has locked own fork\n", 0);
		if (!pthread_mutex_lock(&(philo_->next->fmutex)))
		{
			print_output(philo_, KGRN, "has locked next fork\n", 0);
			print_output(philo_, KYEL, "is eating\n", 0);
			philo_->last_meal = get_time();
			usleep(philo_->data->time_to_eat * 1000);
			philo_->nmeal ++;
			pthread_mutex_unlock(&(philo_->next->fmutex));
			pthread_mutex_unlock(&(philo_->fmutex));
			go_sleep(philo_);
		}
	}
	else
		print_output(philo_, KCYN, "is thinking\n", 0);
}

//La variable death de la structure principale est lock pour être mise à 1
void	death(t_philo *philo)
{
	if (!pthread_mutex_lock(&(philo->data->dmutex)))
	{
		philo->data->death = 1;
		pthread_mutex_unlock(&(philo->data->dmutex));
		print_output(philo, KRED, "died\n", 0);
	}
}
