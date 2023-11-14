/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/14 12:48:05 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	time_to_take_the_fork(t_philo *philo)
{
	unsigned long long	current_time;

	if (pthread_mutex_lock(&philo->fork) == 0)
	{
		if (pthread_mutex_lock(&philo->right_philo->fork) == 0)
		{
			if (philo->fork_state == 0 && philo->right_philo->fork_state == 0
				&& philo->infos->dead_philo == 0)
			{
				current_time = find_time() - philo->infos->start_time;
				printf("%lld %d has taken a fork\n", current_time, philo->no);
				printf("%lld %d has taken a fork\n", current_time, philo->no);
				printf("%lld %d is eating\n", current_time, philo->no);
				philo->fork_state = 1;
				philo->my_turn = 1;
			}
			pthread_mutex_unlock(&philo->right_philo->fork);
		}
		pthread_mutex_unlock(&philo->fork);
	}
}

void	time_to_think(t_philo *philo)
{
	unsigned long long	current_time;

	while (philo->my_turn == 0 && philo->philo_state == 0)
{
		time_to_take_the_fork(philo);
		if (philo->my_turn == 0 && philo->thinking == 0
			&& philo->infos->dead_philo == 0)
		{
			current_time = find_time() - philo->infos->start_time;
			printf("%lld %d is thinking\n",
				current_time, philo->no);
			philo->thinking = 1;
		}
		if (philo->infos->dead_philo != 0)
			return ;
		check_death(philo);
	}
	philo->thinking = 0;
}

void	time_to_eat(t_philo *philo)
{
	unsigned long long	current_time;
	unsigned long long	clean;
	unsigned long long	death;

	current_time = find_time() - philo->infos->start_time;
	clean = current_time + (unsigned long long)philo->infos->time_to_eat;
	death = philo->last_meal + (unsigned long long)philo->infos->time_to_die;
	if (philo->infos->dead_philo == 1)
		return ;
	if (clean > death)
	{
		usleep(((int)current_time + philo->infos->time_to_eat
		- (int)philo->last_meal - philo->infos->time_to_die) * 1000);
		time_to_die(philo, (int)death);
	}
	if (philo->infos->dead_philo != 0)
		return ;
	else
	{
		philo->meal_eaten++;
		while(current_time < clean)
			current_time = find_time() - philo->infos->start_time;
		philo->last_meal = current_time + (unsigned long long)philo->infos->time_to_eat;
		if (philo->meal_eaten == philo->infos->must_eat)
			philo->finished = 1;
		while (philo->my_turn == 1)
		{
			if (pthread_mutex_lock(&philo->fork) == 0)
			{
				philo->my_turn = 0;	
				philo->fork_state = 0;
				pthread_mutex_unlock(&philo->fork);
			}
		}
	}
}

void	time_to_sleep(t_philo *philo)
{
	unsigned long long	current_time;
	unsigned long long	wake_up;
	unsigned long long	death;

	if (philo->infos->dead_philo == 1)
		return ;
	current_time = find_time() - philo->infos->start_time;
	wake_up = current_time + (unsigned long long)philo->infos->time_to_sleep;
	death = philo->last_meal + (unsigned long long)philo->infos->time_to_die;
	if (wake_up > death)
		time_to_die(philo, ((int)death));
	else
	{
		if(philo->finished == 0)
			printf("%lld %d is sleeping\n", current_time, philo->no);
		while(current_time < wake_up)
			current_time = find_time() - philo->infos->start_time;
	}
}

void	time_to_die(t_philo *philo, int t_time)
{
	while (philo->infos->dead_philo == 0)
	{
		if (pthread_mutex_lock(&philo->end) == 0)
		{
			if (philo->infos->dead_philo == 0)
			{
				philo->infos->time_of_death = t_time;
				philo->infos->dead_philo = philo->no;
			}
			pthread_mutex_unlock(&philo->end);
		}
	}
	philo->philo_state = 1;
}

