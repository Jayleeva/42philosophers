/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/24 15:20:33 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	a_sleep(t_philo *philo)
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

void	try_eating(t_philo *philo_)
{
	if (!pthread_mutex_lock(&(philo_->fmutex)))
	{
		philo_->fork_free = FALSE;
		print_output(philo_, KGRN, "has locked own fork\n", 0);
		if (!pthread_mutex_lock(&(philo_->next->fmutex)))
		{
			philo_->next->fork_free = FALSE;
			print_output(philo_, KGRN, "has locked next fork\n", 0);
			print_output(philo_, KYEL, "is eating\n", 0);
			philo_->last_meal = get_time();
			usleep(philo_->data->time_to_eat * 1000);
			philo_->nmeal ++;
			pthread_mutex_unlock(&(philo_->next->fmutex));
			philo_->next->fork_free = TRUE;
			pthread_mutex_unlock(&(philo_->fmutex));
			philo_->fork_free = TRUE;
			a_sleep(philo_);
		}
	}
	else
		print_output(philo_, KCYN, "is thinking\n", 0);
}

void	*routine(void *philo)
{
	t_philo	*philo_;

	philo_ = (t_philo *)philo;
	if (philo_->data->nphilo == 1)
		philo_->next->fork_free = FALSE;
	philo_->last_meal = get_time();
	if (philo_->id % 2 == 0)
		a_sleep(philo_);
	while (!has_ended(philo_->data))
	{
		if (get_time() - philo_->last_meal >= philo_->data->time_to_die)
		{
			philo_->data->death = 1;
			print_output(philo_, KRED, "died\n", 0);
			return (philo_);
		}
		if (philo_->fork_free == TRUE && philo_->next->fork_free == TRUE)
			try_eating(philo_);
	}
	return (philo_);
}

int	start_simulation(t_data *data, pthread_t **thread, t_philo *list)
{
	int	i;

	printf("%s\n========================\n- START OF SIMULATION -\n========================\n", KNRM);
	i = 0;
	while (i < data->nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return (1);
		if (pthread_create(thread[i], NULL, routine, list))
			return (1);
		list = list->next;
		list->data = data;
		data->list = data->list->next;
		i ++;
	}
	return (0);
}

void	end_simulation(t_data *data, pthread_t **thread)
{
	int		i;
	t_philo	*current;

	i = 0;
	current = data->list;
	while (i < data->nphilo)
	{
		pthread_join(*thread[i], NULL);
		pthread_mutex_destroy(&(current->fmutex));
		current = current->next;
		i ++;
	}
	pthread_mutex_destroy(&(data)->mmutex);
	pthread_mutex_destroy(&(data)->pmutex);
	printf("%s\n==========================\n--- END OF SIMULATION ---\n==========================\n", KNRM);
	free_all(data, thread);
}
