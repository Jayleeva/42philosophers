#include "../inc/philosophers.h"

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