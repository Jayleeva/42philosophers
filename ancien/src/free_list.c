#include "../inc/philosophers.h"

//Libere la memoire allouee pour la liste chainee contenant les philo
void	free_list(t_philo *head)
{
	t_philo	*current;

	while (head)
	{
		current = head;
		while (current->next)
			current = current->next;
		free(current);
	}
}