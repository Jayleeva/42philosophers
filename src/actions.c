#include "../inc/philosophers.h"

/*void	wait_end_action(int time_to)
{
	size_t	end;

	end = get_time() + (size_t)time_to;
	while (get_time() < end)
		continue;
}*/

t_bool	is_dead(t_data *data)
{
	if (get_time() - data->list->last_meal >= (size_t)data->time_to_die)
	{
		data->list->dead = TRUE;
		write_output(&data->mutex, KRED, data->list->ID, DEAD);
		//printf("%s%ld %d is dead\n", KRED, get_time(), data->list->ID);
		data->dead = TRUE;
		return (TRUE);
	}
	return (FALSE); 
}

int	a_think(t_data *data)
{
	if (is_dead(data))
		return (1);
	data->list->action = 't';
	pthread_mutex_lock(&(data->mutex).fork0m);
	pthread_mutex_lock(&(data->mutex).fork1m);
	if (data->list->fork_free == TRUE && data->list->next->fork_free == TRUE)
	{
		a_eat(data);
	}
	else
		write_output(&data->mutex, KYEL, data->list->ID, THINK);
		//printf("%s%ld %d is thinking\n", KYEL, get_time(), data->list->ID);
	return (0);
}

int	a_eat(t_data *data)
{
	if (is_dead(data))
		return (1);
	data->list->action = 'e';
	data->list->fork_free = FALSE;
	data->list->next->fork_free = FALSE;
	write_output(&data->mutex, KGRN, data->list->ID, EAT);
	//printf("%s%ld %d is eating\n", KGRN, get_time(), data->list->ID);
	usleep(data->time_to_eat * 1000);
	//wait_end_action(data->time_to_eat);
	data->list->last_meal = get_time();
	data->list->fork_free = TRUE;
	data->list->next->fork_free = TRUE;
	data->list->nmeal ++;
	pthread_mutex_unlock(&data->mutex.fork0m);
	pthread_mutex_unlock(&data->mutex.fork1m);
	if (data->meals_min > -1)
	{
		if (data->list->nmeal == data->meals_min)
		{
			write_output(&data->mutex, KNRM, data->list->ID, FINISH);
			//printf("%s%ld %d has finished all her meals\n", KNRM, get_time(), data->list->ID);
			data->nphilo --;
		}
	}
	//if (a_sleep(data))
	//	return (1);
	return (0);
}

int	a_sleep(t_data *data)
{
	if (is_dead(data))
		return (1);
	data->list->action = 's';
	write_output(&data->mutex, KMAG, data->list->ID, SLEEP);
	//printf("%s%ld %d is sleeping\n", KMAG, get_time(), data->list->ID);
	usleep(data->time_to_sleep * 1000);
	//wait_end_action(data->time_to_sleep);
	//if (a_think(data))
	//	return (1);
	return (0);
}