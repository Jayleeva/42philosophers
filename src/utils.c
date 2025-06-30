/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/30 14:50:36 by cyglardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

//Transformation d'une chaîne de char ascii en int
int	ft_atoi(const char *str)
{
	int	i;
	int	n;
	int	result;

	i = 0;
	n = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13)
		|| str[i] == ' ')
		i ++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i +1] <= '0' && str[i +1] >= '9')
			return (0);
		if (str[i] == '-')
			n = n * -1;
		i ++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i ++;
	}
	return (result * n);
}

//Impression des outputs
void	print_output(t_philo *p, char *color, char *msg)
{
	pthread_mutex_lock(&(p->data)->pmutex);
	p->data->msg = msg;
	printf("%s%ld %d %s", color, get_time(p->data), p->id, p->data->msg);
	pthread_mutex_unlock(&(p->data)->pmutex);
}

//Obtenir le temps au moment du lancement
//de la simulation pour un timestamp plus lisible
time_t	get_init_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
}

//Calcul du temps actuel
time_t	get_time(t_data *data)
{
	struct timeval	time;

	if (!pthread_mutex_lock(&(data->tmutex)))
	{
		if (gettimeofday(&time, NULL) == -1)
			write(2, "gettimeofday() error\n", 22);
		pthread_mutex_unlock(&(data->tmutex));
	}
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000 - data->time);
}

//Verification : le philosophe doit-il s'arreter?
int	must_stop(t_philo *philo)
{
	if (!pthread_mutex_lock(&(philo->data->smutex)))
	{
		if (philo->data->stop)
		{
			pthread_mutex_unlock(&(philo->data->smutex));
			return (1);
		}
		pthread_mutex_unlock(&(philo->data->smutex));
	}
	return (0);
}
