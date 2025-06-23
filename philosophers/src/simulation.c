#include "../inc/philosophers.h"

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
}
//checks every philosopher to see if they've eaten enough or if one of them is dead
int	has_ended(t_data *data)
{
	t_philo	*current;
	int		i;
	int		j;

	current = data->list;
	i = 0;
	j = 0;
	while (i < data->nphilo)
	{
		if (get_time() - current->last_meal >= (size_t)data->time_to_die)
		{
			printf("ONE DEAD\n");
			return (1);
		}
		if (data->meals_min > 0 && current->nmeal >= data->meals_min)
			j ++;
		current = current->next;
		i++;
	}
	if (j == i)
	{
		printf("ALL HAVE EATEN ENOUGH\n");
		return (1);
	}
	return (0);
}
//locks the print mutex to print the received output, then unlocks it
void    write_output(t_philo *philo, char *color, char *action, int type)
{
    pthread_mutex_lock(&(philo->data->output)->printm);
    if (type == 0)
        printf("%s%s\n", KNRM, action);
    else
        printf("%s%ld %d %s\n", color, get_time(), philo->ID, action);
    pthread_mutex_unlock(&(philo->data->output)->printm);
}

void	*routine(void *philo)
{
	t_philo	*philo_;

	philo_ = (t_philo *)philo;
	while (!has_ended(philo_->data))
	{
		pthread_mutex_lock(&(philo_->fork)->forkm);
		if (philo_->fork->fork_free)
		{
			philo_->fork->fork_free = FALSE;
			pthread_mutex_lock(&(philo_->next->fork)->forkm);
			if (philo_->next->fork->fork_free)
			{
				philo_->next->fork->fork_free = FALSE;
				write_output(philo_, KGRN, EAT, 0);
				usleep(philo_->data->time_to_eat);
				philo_->next->fork->fork_free = TRUE;
			}
			pthread_mutex_unlock(&(philo_->next->fork)->forkm);
			philo_->fork->fork_free = TRUE;
		}
		pthread_mutex_unlock(&(philo_->fork)->forkm);
		philo_->nmeal ++;
		philo_->last_meal = get_time();
	}
	return (philo_);
}
//creates all the threads, starting their routines
void	start_simulation(t_data *data)
{
	pthread_t	**thread;
	int			i;

	write_output(data->list, KNRM, START, 0);
	thread = (pthread_t **)malloc((data->nphilo + 1) * sizeof(pthread_t *));
	if (!thread)
		return ;
	i = 0;
	while (i < data->nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return;
		if (pthread_create(thread[i], NULL, routine, data->list))
			return ;
		data->list = data->list->next;
		i ++;
	}
}
//joins all the threads and destroys the mutexes
void	end_simulation(t_philo *list)
{
	int	i;

	i = list->data->nphilo;
	while (i > 0)
	{
		pthread_join(list->thread, NULL);
		list = list->next;
		i--;
	}
	write_output(list, KNRM, END, 0);
	pthread_mutex_destroy(&(list->data->ended)->endm);
	pthread_mutex_destroy(&(list->data->output)->printm);
}