#include "../inc/philosophers.h"

void	wait_end_action(int time_to)
{
	size_t	end;

	end = get_time() + (size_t)time_to;
	while (get_time() < end)
		continue;
}

t_bool	is_dead(t_data *data, t_philo *p)
{
	if (get_time() - p->last_meal >= (size_t)data->time_to_die)
	{
		p->dead = TRUE;
		printf("%s%ld %d is dead\n", KRED, get_time(), p->ID);
		return (TRUE);
	}
	return (FALSE); 
}

int	a_think(t_data *data, t_philo *p)
{
	if (is_dead(data, p))
		return (1);
	p->action = 't';
	printf("%s%ld %d is thinking\n", KYEL, get_time(), p->ID);
	if (p->fork_free == FALSE && p->next->fork_free == FALSE)
	{
		//printf("%ld %d is thinking\n", get_time(), p->ID);
		a_think(data, p);
	}
	else
	{
		if (a_eat(data, p))
			return (1);
	}
	return (0);
}

int	a_eat(t_data *data, t_philo *p)
{
	if (is_dead(data, p))
		return (1);
	p->action = 'e';
	p->fork_free = FALSE;
	p->next->fork_free = FALSE;
	printf("%s%ld %d is eating\n", KGRN, get_time(), p->ID);
	//usleep(data->time_to_eat * 1000);
	wait_end_action(data->time_to_eat);
	p->last_meal = get_time();
	p->fork_free = TRUE;
	p->next->fork_free = TRUE;
	p->nmeal ++;
	if (data->meals_min > -1)
	{
		if (p->nmeal == data->meals_min)
		{
			printf("%ld %d has finished all his meals\n", get_time(), p->ID);
			data->nphilo --;
		}
	}
	if (a_sleep(data, p))
		return (1);
	return (0);
}

int	a_sleep(t_data *data, t_philo *p)
{
	if (is_dead(data, p))
		return (1);
	p->action = 's';
	printf("%s%ld %d is sleeping\n", KMAG, get_time(), p->ID);
	//usleep(data->time_to_sleep * 1000);
	wait_end_action(data->time_to_sleep);
	if (a_think(data, p))
		return (1);
	return (0);
}