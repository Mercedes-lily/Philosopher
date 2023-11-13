/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valerie <valerie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/12 16:43:53 by valerie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	time_to_start(philo);
	if (philo->infos->must_eat == 0)
		philo->finished = 1;
	while (philo->philo_state == 0 && philo->finished == 0 && philo->infos->dead_philo == 0)
	{
		time_to_think(philo);
		if (philo->philo_state == 0)
			time_to_eat(philo);
		if (philo->finished == 0 || philo->philo_state == 0)
			time_to_sleep(philo);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_infos	*infos;
	t_philo	*philo;
	int		n;

	n = 1;
	if (argc != 5 && argc != 6)
		return (printf("Enter a valid number of arguments\n"), 1);
	else
	{
		infos = initialize_infos(argv);
		if (!infos)
			return (1);
		philo = create_philo(infos);
		if (!philo)
			return (1);
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
			if (pthread_join(philo->pt_philo, NULL) != 0)
			{
				clear_philo(philo);
				return (printf("Impossible to wait for the thread"), 1);
			}
			philo = philo->right_philo;
			n ++;
		}
		if(philo->infos->dead_philo != 0)
			printf("%lld %d died\n", infos->time_of_death, infos->dead_philo);
		clear_philo(philo);
		free(infos);
	}
	return (0);
}
