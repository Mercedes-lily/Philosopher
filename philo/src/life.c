/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/29 14:10:11 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	eat2(t_philo *philo, t_ull now, t_ull clean)
{
	if (philo->infos->must_eat != -1)
		philo->meal_eaten++;
	while (now < clean)
		now = find_time() - philo->infos->start_time;
	if (philo->meal_eaten == philo->infos->must_eat)
	{
		pthread_mutex_lock(&philo->god->end);
		philo->god->finished++;
		pthread_mutex_unlock(&philo->god->end);
		philo->finished = 1;
	}
}

void	eat(t_philo *philo, t_ull death)
{
	t_ull	current_time;
	t_ull	clean;

	current_time = find_time() - philo->infos->start_time;
	clean = current_time + (t_ull)philo->infos->time_to_eat;
	if (current_time < death)
	{
		state(philo, "has taken a fork", current_time);
		state(philo, "is eating", current_time);
		philo->last_meal = current_time;
		death = philo->last_meal + (t_ull)philo->infos->time_to_die;
	}
	if (clean > death)
		time_to_die(philo, ((int)death));
	else
		eat2(philo, current_time, clean);
}

void	time_to_eat(t_philo *philo)
{
	t_ull	current_time;
	t_ull	death;

	death = philo->last_meal + (t_ull)philo->infos->time_to_die;
	pthread_mutex_lock(&philo->god->end);
	if (philo->god->dead >= 1)
		philo->philo_state = 1;
	pthread_mutex_unlock(&philo->god->end);
	if (philo->philo_state == 1)
		return (time_to_die(philo, ((int)death)));
	pthread_mutex_lock(&philo->fork);
	current_time = find_time() - philo->infos->start_time;
	if (current_time < death)
		state(philo, "has taken a fork", current_time);
	if (philo->infos->number_of_philosophers == 1)
	{
		pthread_mutex_lock(&philo->god->end);
		philo->philo_state = 1;
		pthread_mutex_unlock(&philo->god->end);
		time_to_die(philo, ((int)death));
		pthread_mutex_unlock(&philo->fork);
		return ;
	}
	pthread_mutex_lock(&philo->right_philo->fork);
	eat(philo, death);
	pthread_mutex_unlock(&philo->right_philo->fork);
	pthread_mutex_unlock(&philo->fork);
}

void	time_to_think(t_philo *philo, t_ull time, t_ull wake_up, t_ull death)
{
	while (time < wake_up)
		time = find_time() - philo->infos->start_time;
	pthread_mutex_lock(&philo->god->end);
	if (philo->god->dead >= 1)
	{
		pthread_mutex_unlock(&philo->god->end);
		return ;
	}
	pthread_mutex_unlock(&philo->god->end);
	if (philo->finished == 0 && time < death)
		state(philo, "is thinking", time);
}

void	time_to_sleep(t_philo *philo)
{
	t_ull	current_time;
	t_ull	wake_up;
	t_ull	death;

	pthread_mutex_lock(&philo->god->end);
	if (philo->god->dead >= 1)
		philo->philo_state = 1;
	pthread_mutex_unlock(&philo->god->end);
	if (philo->finished == 1 || philo->philo_state == 1)
		return ;
	current_time = find_time() - philo->infos->start_time;
	wake_up = current_time + (t_ull)philo->infos->time_to_sleep;
	death = philo->last_meal + (t_ull)philo->infos->time_to_die;
	pthread_mutex_lock(&philo->god->end);
	if (philo->god->dead >= 1)
	{
		pthread_mutex_unlock(&philo->god->end);
		return ;
	}
	pthread_mutex_unlock(&philo->god->end);
	if (philo->finished == 0 && current_time < death)
		state(philo, "is sleeping", current_time);
	if (wake_up > death)
		time_to_die(philo, ((int)death));
	else
		time_to_think(philo, current_time, wake_up, death);
}
