#include "../inc/philosophers.h"

void    write_output(t_mutex *mutex, char *color, int philo, char *action)
{
    //(void)mutex;
    pthread_mutex_lock(&mutex->printm);
    if (philo == -1)
        printf("%s%s\n", KNRM, action);
    else
        printf("%s%ld %d %s\n", color, get_time(), philo, action);
    pthread_mutex_unlock(&mutex->printm);
}