#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef unsigned char	t_bool;
# define TRUE 1
# define FALSE 0

/*typedef struct s_fork
{
	t_bool			fork_free;
	pthread_mutex_t	fmutex;
}				t_fork;*/

typedef struct s_philo
{
	int				id;
	t_bool			fork_free;
	pthread_mutex_t	fmutex;
	struct s_philo	*next;
	struct s_data	*data;
}					t_philo;

typedef struct s_target
{
	char			*txt;
	pthread_mutex_t	mutex;
}					t_target;

typedef struct s_output
{
	char			*msg;
	pthread_mutex_t	pmutex;
}					t_output;

typedef struct s_data
{
	int				nphilo;
	int				mission_done;
	char			*txt;
	t_philo			*list;
	t_target		*target;
	t_output		*output;
}					t_data;

void	init_philo(t_philo *current)
{
	static int	i = 1;

	current->id = i;
	current->fork_free = TRUE;
	current->next = NULL;
	if (pthread_mutex_init(&(current->fmutex), NULL) == -1)
	{
		printf("failed mutex init\n");
		return ;
	}
	i ++;
}

t_philo	*create_list(int nphilo)
{
	t_philo	*head;
	t_philo	*current;
	int		i;

	head = (t_philo *)malloc(sizeof(t_philo));
	if (!head)
		return (NULL);
	init_philo(head);
	current = head;
	i = 1;
	while (i < nphilo)
	{
		while (current->next)
			current = current->next;
		current->next = (t_philo *)malloc(sizeof(t_philo));
		if (!current->next)
			return (NULL);
		init_philo(current->next);
		i ++;
	}
	current->next->next = head;
	return (head);
}

int	has_ended(t_data *data)
{
	if (data->mission_done)
	{
		printf("END\n");
		return (1);
	}
	return (0);
}

void	write_output(t_philo *philo, char *msg, int type) 
{
	pthread_mutex_lock(&(philo->data->output)->pmutex);
	philo->data->output->msg = msg;
	if (type == 1)
		printf("%d %s %s\n", philo->id, philo->data->output->msg, philo->data->target->txt);
	else
		printf("%d %s", philo->id, philo->data->output->msg);
	pthread_mutex_unlock(&(philo->data->output)->pmutex);
}

void	*routine(void *philo)
{
	t_philo	*philo_;

	philo_ = (t_philo *)philo;
	write_output(philo_, "entered 'routine'\n", 0);
	while (!has_ended(philo_->data))
	{
		write_output(philo_, "has entered loop\n", 0);
		if (philo_->fork_free == TRUE && philo_->next->fork_free == TRUE)
		{
			if (pthread_mutex_lock(&(philo_->fmutex)))			// besoin de vérifier si possible de lock? que faire si peut pas? attendre?
			{
				write_output(philo_, "couldn't lock own fork\n", 0);
				usleep(1000);
			}	
			philo_->fork_free = FALSE;
			write_output(philo_, "has locked own fork\n", 0);
			if (pthread_mutex_lock(&(philo_->next->fmutex))) 	// comment faire attendre et réessayer de lock?
			{
				write_output(philo_, "couldn't lock next fork\n", 0);
				pthread_mutex_unlock(&(philo_->fmutex));
				philo_->fork_free = TRUE;
				write_output(philo_, "has unlocked own fork\n", 0);
				usleep(1000);
			}
			else
			{
				philo_->next->fork_free = FALSE;
				write_output(philo_, "has locked next fork\n", 0);
				pthread_mutex_lock(&(philo_->data->target)->mutex);
				if (!strncmp(philo_->data->target->txt, "salut", 5))
				{
					write_output(philo_, "BEFORE :", 1);
					philo_->data->target->txt = strdup("adieu");
					philo_->data->mission_done = 1;
					write_output(philo_, "AFTER :", 1);
					write_output(philo_, "--- MISSION DONE ---\n", 0);
					pthread_mutex_unlock(&(philo_->data->target)->mutex);
				}
				pthread_mutex_unlock(&(philo_->next->fmutex));
				philo_->next->fork_free = TRUE;
				write_output(philo_, "has unlocked next fork\n", 0);
				pthread_mutex_unlock(&(philo_->fmutex));
				philo_->fork_free = TRUE;
				write_output(philo_, "has unlocked own fork\n", 0);
			}
		}
		else
			usleep(1000);
	}
	write_output(philo_, "exited loop\n", 0);
	return (philo_);
}

int	main(int argc, char **argv)
{
	int			i;
	pthread_t	**thread;
	t_philo 	*list;
	t_data		data;
	t_target	target;
	t_output	output;

	if (argc != 2)
		return 1;
	data.nphilo = atoi(argv[1]);
	data.mission_done = 0;
	data.output = &output;
	data.target = &target;
	data.target->txt = strdup("salut");
	
	list = create_list(data.nphilo);
	if (!list)
		return (1);
	data.list = list;
	list->data = &data;
	if (pthread_mutex_init(&(data.output)->pmutex, NULL) == -1)
	{
		printf("failed mutex init\n");
		return (1);
	}
	if (pthread_mutex_init(&(data.target)->mutex, NULL) == -1)
	{
		printf("failed mutex init\n");
		return (1);
	}

	thread = (pthread_t **)malloc((data.nphilo + 1) * sizeof(pthread_t *));
	if (!thread)
		return 1;
	i = 0;
	while (i < data.nphilo)
	{
		thread[i] = malloc(sizeof(pthread_t *));
		if (!thread[i])
			return 1;
		if (pthread_create(thread[i], NULL, routine, list))
			return 1;
		list = list->next;
		list->data = &data;
		data.list = data.list->next;
		i ++;
	}
	i = 0;
	t_philo	*current;
	current = data.list;
	while (i < data.nphilo)
	{
		pthread_join(*thread[i], NULL);
		pthread_mutex_destroy(&(current->fmutex));
		current = current->next;
		i ++;
	}
	pthread_mutex_destroy(&(data.target)->mutex);
	pthread_mutex_destroy(&(data.output)->pmutex);
	
	return (0);
}