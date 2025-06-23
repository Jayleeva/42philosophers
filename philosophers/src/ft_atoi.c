#include "../inc/philosophers.h"

//Transformation d'une chaine de char ascii en int
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