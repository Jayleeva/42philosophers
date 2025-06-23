#include "../inc/philosophers.h"

int	has_ended(t_data *data)
{
	t_philo	*current;
	int		i;

	current = data->list;
	if (data->death)
		return (1);
	else
	{
		if (data->minmeals > -1)
		{
			i = 0;
			while (i < data->nphilo)
			{
				if (current->nmeal < data->minmeals)
					return (0);
				current = current->next;
				i ++;
			}
			return (1);
		}
		return (0);
	}
}

void	a_sleep(t_philo *philo)
{
	if (!has_ended(philo->data))
	{
		write_output(philo, KMAG, "is sleeping\n", 0);
		usleep(philo->data->time_to_sleep * 1000);
		if (!has_ended(philo->data))
			write_output(philo, KCYN, "is thinking\n", 0);
	}
	else
		return ;
}

void	try_eating(t_philo *philo_)
{
	if (pthread_mutex_lock(&(philo_->fmutex)))
		usleep(1000);
	philo_->fork_free = FALSE;
	write_output(philo_, KGRN, "has locked own fork\n", 0);
	if (pthread_mutex_lock(&(philo_->next->fmutex)))
	{
		pthread_mutex_unlock(&(philo_->fmutex));
		philo_->fork_free = TRUE;
		usleep(1000);
	}
	else
	{
		philo_->next->fork_free = FALSE;
		write_output(philo_, KGRN, "has locked next fork\n", 0);
		write_output(philo_, KYEL, "is eating\n", 0);
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

void	*routine(void *philo)
{
	t_philo	*philo_;

	philo_ = (t_philo *)philo;
	philo_->last_meal = get_time();
	if (philo_->id % 2 == 0)
		a_sleep(philo_);
	while (!has_ended(philo_->data))
	{
		if (get_time() - philo_->last_meal >= philo_->data->time_to_die)
		{
			philo_->data->death = 1;
			write_output(philo_, KRED, "died\n", 0);
			return (philo_);
		}
		if (philo_->fork_free == TRUE && philo_->next->fork_free == TRUE)
			try_eating(philo_);
		else
			usleep(1000);
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