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
		pthread_mutex_lock(&(data->mutex).deadm);
		data->dead = TRUE;
		pthread_mutex_unlock(&(data->mutex).deadm);
		return (TRUE);
	}
	return (FALSE); 
}

int	a_think(t_data *data)
{
	if (has_simulation_ended(data))
		return (1);
	data->list->action = 't';
	pthread_mutex_lock(&(data->mutex).forkm);
	if (data->list->fork_free == TRUE && data->list->next->fork_free == TRUE)
	{
		if (a_eat(data))
		{
			pthread_mutex_unlock(&(data->mutex).forkm);
			return (1);
		}
	}
	else
		write_output(&data->mutex, KYEL, data->list->ID, THINK);
	pthread_mutex_unlock(&(data->mutex).forkm);
	return (0);
}

int	a_eat(t_data *data)
{
	if (has_simulation_ended(data))
		return (1);
	data->list->action = 'e';
	data->list->fork_free = FALSE;
	data->list->next->fork_free = FALSE;
	write_output(&data->mutex, KGRN, data->list->ID, EAT);
	usleep(data->time_to_eat * 1000);
	//wait_end_action(data->time_to_eat);
	data->list->last_meal = get_time();
	data->list->fork_free = TRUE;
	data->list->next->fork_free = TRUE;
	data->list->nmeal ++;
	if (data->meals_min > -1)
	{
		if (data->list->nmeal == data->meals_min)
		{
			write_output(&data->mutex, KNRM, data->list->ID, FINISH);
			data->nphilo --;
		}
	}
	return (0);
}

int	a_sleep(t_data *data)
{
	if (has_simulation_ended(data))
		return (1);
	data->list->action = 's';
	write_output(&data->mutex, KMAG, data->list->ID, SLEEP);
	usleep(data->time_to_sleep * 1000);
	//wait_end_action(data->time_to_sleep);
	return (0);
}