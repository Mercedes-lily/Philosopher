/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/16 16:26:41 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	eat(t_philo *philo, unsigned long long death)
{
	unsigned long long	current_time;
	unsigned long long	clean;

	current_time = find_time() - philo->infos->start_time;
	clean = current_time + (unsigned long long)philo->infos->time_to_eat;
	printf("%lld %d has taken a fork\n", current_time, philo->no);
	printf("%lld %d is eating\n", current_time, philo->no);
	if (clean > death)
		time_to_die(philo, ((int)death));
	else
	{
		if (philo->infos->must_eat != -1)
			philo->meal_eaten++;
		if (philo->meal_eaten == philo->infos->must_eat)
			philo->finished = 1;
		while (current_time < clean)
			current_time = find_time() - philo->infos->start_time;
	}
}

void	time_to_eat(t_philo *philo)
{
	unsigned long long	current_time;
	unsigned long long	death;

	if (philo->finished == 1)
	{
		return ;
	}
	death = philo->last_meal + (unsigned long long)philo->infos->time_to_die;
	pthread_mutex_lock(&philo->fork);
	current_time = find_time() - philo->infos->start_time;
	printf("%lld %d has taken a fork\n", current_time, philo->no);
	if (philo->infos->number_of_philosophers == 1)
		time_to_die(philo, ((int)death));
	pthread_mutex_lock(&philo->right_philo->fork);
	eat(philo, death);
	pthread_mutex_unlock(&philo->right_philo->fork);
	pthread_mutex_unlock(&philo->fork);
}

void	time_to_sleep(t_philo *philo)
{
	unsigned long long	current_time;
	unsigned long long	wake_up;
	unsigned long long	death;

	if (philo->finished == 1 || philo->philo_state == 1)
		return ;
	current_time = find_time() - philo->infos->start_time;
	wake_up = current_time + (unsigned long long)philo->infos->time_to_sleep;
	death = philo->last_meal + (unsigned long long)philo->infos->time_to_die;
	if (philo->finished == 0)
		printf("%lld %d is sleeping\n", current_time, philo->no);
	if (wake_up > death)
		time_to_die(philo, ((int)death));
	else
	{
		while (current_time < wake_up)
			current_time = find_time() - philo->infos->start_time;
		if (philo->finished == 0)
			printf("%lld %d is sleeping\n", current_time, philo->no);
	}
}

void	time_to_die(t_philo *philo, int time)
{
	philo->philo_state = 1;
	philo->time_of_death = time;
}
