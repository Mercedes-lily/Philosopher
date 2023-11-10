/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:48 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/10 15:53:27 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	time_to_start(philo);
	if (philo->infos->must_eat == 0)
		philo->finished = 1;
	while (philo->philo_state == 0 && philo->finished == 0)
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
	if (argc != 4 && argc != 5)
		return (printf("Enter a valid number of arguments\n"), 1);
	else
	{
		infos = initialize_infos(argv);
		if (!infos)
			return (printf("Malloc Error\n"), 1);
		philo = create_philo(infos);
		if (!philo)
			return (1);
		dprintf(2, "hi");
		while (n <= infos->number_of_philosophers)
		{
			pthread_create(&philo->pt_philo, NULL, &routine, philo);
			philo = philo->right_philo;
			n ++;
		}
		n = 1;
		while (n <= infos->number_of_philosophers)
		{
			pthread_join(philo->pt_philo, NULL);
			philo = philo->right_philo;
			n ++;
		}
		free(infos);
		clear_philo(philo);
		printf("%lld Philo %d is sleeping", philo->infos->time_of_death, philo->infos->dead_philo);
	}
	return (0);
}
