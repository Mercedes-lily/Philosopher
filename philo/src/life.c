/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/10 15:58:18 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	time_to_take_the_fork(t_philo *philo)
{
	long long	current_time;

	if (pthread_mutex_lock(&philo->fork) == 0)
	{
		if (pthread_mutex_lock(&philo->right_philo->fork) == 0)
		{
			if (philo->fork_state == 0 && philo->right_philo->fork_state == 0)
			{
				current_time = find_time() - philo->infos->start_time;
				printf("%lld Philo %d has taken a fork",
					current_time, philo->no);
				philo->fork_state = 1;
				philo->right_philo->fork_state = 1;
				philo->my_turn = 1;
			}
			pthread_mutex_unlock(&philo->right_philo->fork);
		}
		pthread_mutex_unlock(&philo->right_philo->fork);
	}
}

void	time_to_think(t_philo *philo)
{
	long long	current_time;

	while (philo->my_turn == 0)
	{
		check_death(philo);
		if (philo->philo_state == 0)
		{
			time_to_take_the_fork(philo);
			if (philo->my_turn == 0 && philo->thinking == 0)
			{
				current_time = find_time() - philo->infos->start_time;
				printf("%lld Philo %d has taken  is thinking",
					current_time, philo->no);
				philo->thinking = 1;
			}
		}
	}
	philo->thinking = 0;
}

void	time_to_eat(t_philo *philo)
{
	long long	current_time;
	long long	time;

	time = find_time();
	current_time = time - philo->infos->start_time;
	printf("%lld Philo %d is eating", current_time, philo->no);
	time = current_time - philo->last_meal + philo->infos->time_to_eat ;
	if (time >= philo->infos->time_to_die)
	{
		usleep((int)(philo->infos->time_to_die
				- (current_time + philo->last_meal)));
		time_to_die(philo);
	}
	else
	{
		philo->meal_eaten++;
		philo->my_turn = 0;
		usleep(philo->infos->time_to_eat);
		philo->last_meal = current_time + philo->infos->time_to_eat;
		if (philo->meal_eaten == philo->infos->must_eat)
			philo->finished = 1;
	}
}

void	time_to_sleep(t_philo *philo)
{
	long long	current_time;
	long long	time;

	time = find_time();
	current_time = time - philo->infos->start_time;
	printf("%lld Philo %d is sleeping", current_time, philo->no);
	time = time + philo->infos->time_to_sleep - philo->last_meal;
	if (time >= philo->infos->time_to_die)
	{
		usleep((int)(philo->infos->time_to_die
				- (current_time + philo->last_meal)));
		time_to_die(philo);
	}
	else
		usleep(philo->infos->time_to_sleep);
}

void	time_to_die(t_philo *philo)
{
	long long	current_time;

	current_time = find_time();
	while (philo->no == 0)
	{
		if (pthread_mutex_lock(&philo->end) == 0)
		{
			if (philo->infos->dead_philo == 0)
			{
				philo->infos->time_of_death = current_time
						- philo->infos->start_time;
				philo->infos->dead_philo = philo->no;
			}
			pthread_mutex_unlock(&philo->end);
		}
	}
	printf("%lld Philo %d died", current_time, philo->no);
	philo->philo_state = 1;
}
