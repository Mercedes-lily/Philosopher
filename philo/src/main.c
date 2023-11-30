/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/30 14:57:08 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->no % 2 == 0)
		usleep(15000);
	while (1)
	{
		time_to_eat(philo);
		if (is_it_finished(philo) == 1)
			return (0);
		time_to_sleep(philo);
		if (is_it_finished(philo) == 1)
			return (0);
	}
	return (0);
}

void	god_function(t_god *god)
{
	t_ull	current_time;
	t_ull	death;

	if (god->philo->infos->must_eat == 0)
	{
		pthread_mutex_lock(&god->end);
		god->finished = god->philo->infos->number_of_philosophers;
		pthread_mutex_unlock(&god->end);
	}
	while (god->finished != god->philo->infos->number_of_philosophers)
	{
		pthread_mutex_lock(&god->end);
		current_time = find_time() - god->philo->infos->start_time;
		death = god->philo->last_meal + (t_ull)god->philo->infos->time_to_die;
		if (death <= current_time
			|| god->philo->infos->number_of_philosophers == 1)
		{
			god->finished = god->philo->infos->number_of_philosophers;
			god->first_death_philo = god->philo->no;
			god->time_of_death = death;
			pthread_mutex_unlock(&god->end);
			return ;
		}
		if (god->philo->infos->must_eat == god->philo->meal_eaten
			&& god->philo->finished != 1)
		{
			god->philo->finished = 1 ;
			god->finished++ ;
			pthread_mutex_unlock(&god->end);
			return ;
		}
		pthread_mutex_unlock(&god->end);
		god->philo = god->philo->right_philo;
	}
}

int	life_of_philosopher(t_philo *philo, t_infos *infos)
{
	int	n;

	n = 1;
	while (n <= infos->number_of_philosophers)
	{
		if (pthread_create(&philo->pt_philo, NULL, &routine, philo) != 0)
		{
			clear_philo(philo);
			return (printf("Error during the thread creation"), 1);
		}
		philo = philo->right_philo;
		n ++;
	}
	n = 1;
	while (n <= infos->number_of_philosophers)
	{
		if (pthread_detach(philo->pt_philo) != 0)
		{
			clear_philo(philo);
			return (printf("Impossible to wait for the thread"), 1);
		}
		philo = philo->right_philo;
		n ++;
	}
	return (0); 
}

void	free_all(t_philo *philo, t_infos *infos, t_god *god)
{
	clear_philo(philo);
	free(infos);
	infos = NULL;
	free(god);
	god = NULL;
}

int	main(int argc, char **argv)
{
	t_infos	*infos;
	t_philo	*philo;
	t_god	*god;

	if (argc != 5 && argc != 6)
		return (printf("Enter a valid number of arguments\n"), 1);
	else
	{
		infos = initialize_infos(argv);
		if (!infos)
			return (1);
		god = initialize_god(infos);
		if (!god)
		{
			free(infos);
			return (1);
		}
		philo = create_philo(infos, god);
		if (!philo)
		{
			free(infos);
			free(god);
			return (1);
		}
		god->philo = philo;
		infos->start_time = find_time();
		if (life_of_philosopher(philo, infos) == 1)
			return (1);
		god_function(god);
		if (god->first_death_philo != 0)
		{
			pthread_mutex_lock(&philo->god->printf);
			printf("%lld %d died\n", god->time_of_death, god->first_death_philo);
			pthread_mutex_unlock(&philo->god->printf);
		}
		free_all(philo, infos, god);
	}
	return (0);
}
