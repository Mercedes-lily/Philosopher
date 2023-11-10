/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/10 14:29:49 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_positive_atoi(const char *str)
{
	int		i;
	long	nb;

	i = 0;
	nb = 0;
	if (!str)
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10;
		nb = nb + str[i] - 48;
		i++;
	}
	if (nb > MAX_INPUT)
		return (printf("Bigger then 2147483647"), -2);
	return (nb);
}

void	clear_philo(t_philo *philo)
{
	t_philo	*philo_copy;

	while (philo)
	{
		pthread_mutex_destroy(&philo->fork);
		philo_copy = philo;
		if (philo->right_philo)
			philo = philo->right_philo;
		free(philo_copy);
		philo_copy = NULL;
	}
}

long long	find_time(void)
{
	struct timeval	*tv;
	long long		micro_time;

	tv = NULL;
	gettimeofday(tv, NULL);
	micro_time = (((long long)tv->tv_sec * 1000)
			+ (long long)tv->tv_usec / 1000);
	return (micro_time);
}
