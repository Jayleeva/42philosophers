#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct s_philo
{
	int				id;
	struct s_philo	*next;
	//pthread_mutex_t	imutex;
	struct s_data	*data;
}					t_philo;

/*typedef struct s_list
{
	t_philo			*philo;
	pthread_mutex_t	*lmutex;
}					t_list;*/

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
	//char			*txt;
	t_philo			*list;
	t_target		*target;
	t_output		*output;
}					t_data;

void	init_philo(t_philo *current)
{
	static int	i = 1;

	current->id = i;
	current->next = NULL;
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
	//if (!strncmp(data->target->txt, "hello", 5))
		return (1);
	return (0);
}

void	write_output(t_data *data, char *msg, int type) 
{
	pthread_t	tid;

	tid = pthread_self();
	pthread_mutex_lock(&(data->output)->pmutex);
	data->output->msg = msg;
	if (type == 1)
		printf("%ld %s %s\n", tid, data->output->msg, data->target->txt);
	else
		printf("%ld %s", tid, data->output->msg);
	pthread_mutex_unlock(&(data->output)->pmutex);
}

void	*routine(void *data)
{
	t_data	*data_;

	data_ = (t_data *)data;
	write_output(data_, "entered 'routine'\n", 0);
	while (!has_ended(data_))
	{
		write_output(data_, "has entered loop\n", 0);
		pthread_mutex_lock(&(data_->target)->mutex);
		write_output(data_, "has locked target\n", 0);
		
		if (!strncmp(data_->target->txt, "salut", 5))
		{
			write_output(data_, "BEFORE :", 1);
			data_->target->txt = strdup("adieu");
			data_->mission_done = 1;
			write_output(data_, "AFTER :", 1);
			write_output(data_, "--- MISSION DONE ---\n", 0);
		}
		pthread_mutex_unlock(&(data_->target)->mutex);
		write_output(data_, "has unlocked target\n", 0);	
	}
	write_output(data_, "exited loop\n", 0);
	return (data);
}

int	main(void)
{
	t_data		data;
	t_philo		*list;
	t_target	target;
	t_output	output;

	pthread_t	thread0;
	pthread_t	thread1;
	pthread_t	thread2;

	data.nphilo = 3;
	data.target = &target;
	data.output = &output;
	data.target->txt = strdup("salut");
	data.mission_done = 0;
	list = create_list(data.nphilo);
	if (!list)
		return (1);
	data.list = list;

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
	if (pthread_create(&thread0, NULL, routine, &data))
		return (1);
	if (pthread_create(&thread1, NULL, routine, &data))
		return (1);
	if (pthread_create(&thread2, NULL, routine, &data))
		return (1);
	
	pthread_join(thread0, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_mutex_destroy(&(data.target)->mutex);
	pthread_mutex_destroy(&(data.output)->pmutex);
	return (0);
}