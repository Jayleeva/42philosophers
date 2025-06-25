/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyglardo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:27:20 by cyglardo          #+#    #+#             */
/*   Updated: 2025/06/23 15:37:46 by cyglardo         ###   ########.fr       */
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

//Libération de toute la mémoire allouée
void	free_all(t_data *data, pthread_t **thread)
{
	int		i;
	t_philo	*current;
	t_philo	*temp;

	free(thread);
	i = 0;
	current = data->list;
	while (i < data->nphilo)
	{
		temp = current;
		current = current->next;
		free(temp);
		i ++;
	}
}

//Impression des outputs
void	print_output(t_philo *philo, char *color, char *msg, int type)
{
	pthread_mutex_lock(&(philo->data)->pmutex);
	philo->data->msg = msg;
	if (type == 1)
		printf("%s%s\n", color, philo->data->msg);
	else
		printf("%s%ld %d %s", color, get_time(), philo->id, philo->data->msg);
	pthread_mutex_unlock(&(philo->data)->pmutex);
}

//Calcul du temps actuel
time_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec) * 1000 + (time.tv_usec) / 1000);
}
