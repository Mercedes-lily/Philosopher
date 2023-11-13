/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valerie <valerie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/13 16:43:17 by valerie          ###   ########.fr       */
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
	infos->dead_philo = 0;
	infos->time_of_death = -1;
	infos->start_time = 0;
	return (infos);
}

t_philo	*initialize_philo(t_infos *infos, int no)
{
	t_philo	*philo;

	philo = ft_calloc(1, sizeof(t_philo));
	if (!philo)
		return (free(infos), NULL);
	philo->no = no;
	philo->philo_state = 0;
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
		return (free(infos), NULL);
	if (pthread_mutex_init(&philo->start, NULL) != 0)
		return (free(infos), NULL);
	if (pthread_mutex_init(&philo->end, NULL) != 0)
		return (free(infos), NULL);
	philo->fork_state = 0;
	philo->finished = 0;
	philo->meal_eaten = -1;
	philo->infos = infos;
	philo->right_philo = NULL;
	philo->my_turn = 0;
	philo->thinking = 0;
	philo->last_meal = 0;
	return (philo);
}

t_philo	*create_philo(t_infos *infos)
{
	t_philo	*philo;
	t_philo	*next;
	t_philo	*head;
	int		no;

	no = 1;
	philo = initialize_philo(infos, no);
	head = philo;
	no++;
	while (no <= infos->number_of_philosophers)
	{
		next = initialize_philo(infos, no);
		philo->right_philo = next;
		philo = philo->right_philo;
		no++;
	}
	philo->right_philo = head;
	return (head);
}

void	time_to_start(t_philo *philo)
{
	while (philo->meal_eaten == -1)
	{
		if (pthread_mutex_lock(&philo->start) == 0)
		{
			philo->meal_eaten = 0;
			if (philo->infos->start_time == 0)
				philo->infos->start_time = find_time();
			pthread_mutex_unlock(&philo->start);
		}
	}
	philo->last_meal = 0;
}
