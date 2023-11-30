/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/30 13:56:25 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

t_infos	*initialize_infos(char **argv)
{
	t_infos	*infos;

	infos = ft_calloc(1, sizeof(t_infos));
	if (!infos)
		return (printf("Malloc Error\n"), NULL);
	if (check_is_number(argv) == 1)
		return (NULL);
	infos->number_of_philosophers = ft_positive_atoi(argv[1]);
	infos->time_to_die = ft_positive_atoi(argv[2]);
	infos->time_to_eat = ft_positive_atoi(argv[3]);
	infos->time_to_sleep = ft_positive_atoi(argv[4]);
	if (!argv[5])
		infos->must_eat = -1;
	else
		infos->must_eat = ft_positive_atoi(argv[5]);
	if (infos->number_of_philosophers == -2 || infos->time_to_die == -2
		|| infos->time_to_eat == -2 || infos->time_to_sleep == -2
		|| infos->must_eat == -2)
		return (NULL);
	infos->start_time = 0;
	return (infos);
}

t_god	*initialize_god(t_infos *infos)
{
	t_god	*god;

	god = ft_calloc(1, sizeof(t_god));
	god->first_death_philo = 0;
	god->time_of_death = 0;
	if (pthread_mutex_init(&god->end, NULL) != 0)
	{
		free(infos);
		return (NULL);
	}
	if (pthread_mutex_init(&infos->start, NULL) != 0)
	{
		pthread_mutex_destroy(&god->end);
		free(infos);
		return (NULL);
	}
	if (pthread_mutex_init(&god->printf, NULL) != 0)
	{
		pthread_mutex_destroy(&god->end);
		pthread_mutex_destroy(&infos->start);
		free(infos);
		return (NULL);
	}
	return (god);
}

t_philo	*initialize_philo(t_infos *infos, t_god *god, int no)
{
	t_philo	*philo;

	philo = ft_calloc(1, sizeof(t_philo));
	if (!philo)
		return (free(infos), NULL);
	philo->no = no;
	philo->philo_state = 0;
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
	{
		free(philo);
		free(god);
		free(infos);
		return (NULL);
	}
	philo->finished = 0;
	philo->meal_eaten = 0;
	philo->infos = infos;
	philo->right_philo = NULL;
	philo->thinking = 0;
	philo->last_meal = 0;
	philo->time_of_death = 0;
	philo->god = god;
	return (philo);
}

t_philo	*create_philo(t_infos *infos, t_god *god)
{
	t_philo	*philo;
	t_philo	*head;
	int		no;

	no = 1;
	philo = initialize_philo(infos, god, no++);
	head = philo;
	while (no <= infos->number_of_philosophers)
	{
		philo->right_philo = initialize_philo(infos, god, no++);
		philo = philo->right_philo;
	}
	philo->right_philo = head;
	return (head);
}
