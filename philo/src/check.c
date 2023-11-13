/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valerie <valerie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/12 15:46:34 by valerie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_is_number(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (ft_isdigit(argv[i][j]) == 0)
			{
				printf("Non-digit value find in %s\n", argv[i]);
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

void	check_death(t_philo *philo)
{
	unsigned long long	current_time;
	unsigned long long	time;

	time = find_time();
	current_time = time - philo->infos->start_time;
	if (current_time >= philo->last_meal + philo->infos->time_to_die)
		time_to_die(philo, (int)philo->last_meal + philo->infos->time_to_die);
}
