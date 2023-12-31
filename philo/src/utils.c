/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/27 15:34:08 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_positive_atoi(const char *str)
{
	int				i;
	unsigned long	nb;

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
	if (nb > INT_MAX)
		return (printf("Bigger then 2147483647\n"), -2);
	return (nb);
}

void	clear_philo(t_philo *philo)
{
	t_philo	*philo_copy;
	int		i;

	i = 1;
	while (i < philo->infos->number_of_philosophers)
	{
		pthread_mutex_destroy(&philo->fork);
		philo_copy = philo;
		philo = philo->right_philo;
		free(philo_copy);
		philo_copy = NULL;
		i++;
	}
	pthread_mutex_destroy(&philo->fork);
	pthread_mutex_destroy(&philo->god->end);
	pthread_mutex_destroy(&philo->god->printf);
	pthread_mutex_destroy(&philo->infos->start);
	pthread_mutex_destroy(&philo->fork);
	free(philo);
	philo = NULL;
}

t_ull	find_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	*ft_calloc(size_t count, size_t size)
{
	char			*temp;
	void			*copy;
	unsigned long	i;

	i = 0;
	copy = (void *)malloc(count * size);
	if (copy == NULL)
		return (NULL);
	temp = (char *)copy;
	while (i < size * count)
	{
		temp[i] = 0;
		i++;
	}
	return (copy);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (c);
	return (0);
}
