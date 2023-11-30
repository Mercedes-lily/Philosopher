/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/30 15:04:50 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	is_it_finished(t_philo *philo)
{
	pthread_mutex_lock(&philo->god->end);
	if (philo->god->finished == philo->infos->number_of_philosophers
		|| philo->finished == 1)
	{
		pthread_mutex_unlock(&philo->god->end);
		return (1);
	}
	pthread_mutex_unlock(&philo->god->end);
	return (0);
}

void	eat(t_philo *philo)
{
	t_ull	current_time;
	t_ull	clean;

	current_time = find_time() - philo->infos->start_time;
	clean = current_time + (t_ull)philo->infos->time_to_eat;
	if (is_it_finished(philo) == 1)
		return ;
	state(philo, "has taken a fork", current_time);
	state(philo, "is eating", current_time);
	pthread_mutex_lock(&philo->god->end);
	philo->last_meal = current_time;
	if (philo->infos->must_eat != -1)
		philo->meal_eaten++;
	pthread_mutex_unlock(&philo->god->end);
	while (current_time < clean)
		current_time = find_time() - philo->infos->start_time;
}

	//god->philo->finished = 1 ;
	//god->finished = number philo ;

void	time_to_eat(t_philo *philo)
{
	t_ull	current_time;

	if (is_it_finished(philo) == 1)
		return ;
	pthread_mutex_lock(&philo->fork);
	if (is_it_finished(philo) == 1)
		return ;
	current_time = find_time() - philo->infos->start_time;
	state(philo, "has taken a fork", current_time);
	if (philo->infos->number_of_philosophers == 1)
	{
		pthread_mutex_unlock(&philo->fork);
		return ;
	}
	pthread_mutex_lock(&philo->right_philo->fork);
	eat(philo);
	pthread_mutex_unlock(&philo->right_philo->fork);
	pthread_mutex_unlock(&philo->fork);
}

void	time_to_sleep(t_philo *philo)
{
	t_ull	current_time;
	t_ull	wake_up;

	current_time = find_time() - philo->infos->start_time;
	wake_up = current_time + (t_ull)philo->infos->time_to_sleep;
	printf("-%llu- -%llu- -%llu-\n", current_time, wake_up, philo->infos->start_time);
	if (is_it_finished(philo) == 1)
		return ;
	state(philo, "is sleeping", current_time);
	while (current_time < wake_up)
		current_time = find_time() - philo->infos->start_time;
	if (is_it_finished(philo) == 1)
		return ;
	state(philo, "is thinking", current_time);
}
