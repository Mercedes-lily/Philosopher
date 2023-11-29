/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/29 14:13:33 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	time_to_start(philo);
	if (philo->infos->must_eat == 0)
	{
		pthread_mutex_lock(&philo->god->end);
		philo->god->finished++;
		pthread_mutex_unlock(&philo->god->end);
		philo->finished = 1;
	}
	if (philo->no % 2 == 0)
		usleep(15000);
	while (philo->philo_state == 0 && philo->finished == 0)
	{
		time_to_eat(philo);
		pthread_mutex_lock(&philo->god->end);
		if (philo->infos->number_of_philosophers == 1
			|| philo->philo_state == 1
			|| philo->finished == 1 || philo->god->dead >= 1)
		{
			pthread_mutex_unlock(&philo->god->end);
			return (0);
		}
		pthread_mutex_unlock(&philo->god->end);
		time_to_sleep(philo);
		pthread_mutex_lock(&philo->god->end);
		if (philo->god->dead >= 1)
		{
			pthread_mutex_unlock(&philo->god->end);
			return (0);
		}
		pthread_mutex_unlock(&philo->god->end);
	}
	return (0);
}

void	*god_function(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (philo->god->first_death_philo == 0)
	{
		pthread_mutex_lock(&philo->god->end);
		if (philo->philo_state == 1)
		{
			philo->god->time_of_death = philo->time_of_death;
			philo->god->first_death_philo = philo->no;
		}
		if (philo->god->finished == philo->infos->number_of_philosophers)
		{
			pthread_mutex_unlock(&philo->god->end);
			return (0);
		}
		pthread_mutex_unlock(&philo->god->end);
		philo = philo->right_philo;
	}
	return (0);
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
	free(god);
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
			return (1);
		philo = create_philo(infos, god);
		if (!philo)
			return (1);
		if (life_of_philosopher(philo, infos) == 1)
			return (1);
		god_function(philo);
		if (god->first_death_philo != 0)
			printf("%lld %d died\n", god->time_of_death, god->first_death_philo);
		free_all(philo, infos, god);
	}
	return (0);
}
