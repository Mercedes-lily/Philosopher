/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:12:49 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/21 15:15:31 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	lock(t_philo *philo)
{
	pthread_mutex_lock(&philo->god->end);
	if (philo->god->dead > 1)
		philo->philo_state = 1;
	pthread_mutex_unlock(&philo->god->end);
}

void	time_to_die(t_philo *philo, int time)
{
	int	current_time;

	pthread_mutex_lock(&philo->god->end);
	philo->philo_state = 1;
	philo->time_of_death = time;
	philo->god->dead++;
	pthread_mutex_unlock(&philo->god->end);
	current_time = find_time() - philo->infos->start_time;
	usleep((time - current_time) * 1000);
}
