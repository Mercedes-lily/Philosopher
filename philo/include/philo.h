/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 12:57:27 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/15 12:04:40 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <limits.h>
# include <sys/time.h>
# include "../libft/libft.h"

typedef struct s_god
{
	struct s_philo		*philo;
	int					first_death_philo;
	unsigned long long	time_of_death;
	int					nb_of_death_philosophers;
}	t_god;

typedef struct s_infos
{
	int					number_of_philosophers;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					must_eat;
	unsigned long long	start_time;
}	t_infos;

typedef struct s_philo
{
	pthread_t				pt_philo;
	int						no;
	int						philo_state;
	int						finished;
	int						meal_eaten;
	struct s_infos			*infos;
	struct s_philo			*right_philo;
	int						thinking;
	unsigned long long		last_meal;
	unsigned long long		time_of_death;
	pthread_mutex_t			fork;
	pthread_mutex_t			start;
}	t_philo;

int					check_is_number(char **argv);
void				check_death(t_philo *philo);
t_infos				*initialize_infos(char **argv);
t_philo				*initialize_philo(t_infos *infos, int no);
t_god				*initialize_god(t_philo *philo);
t_philo				*create_philo(t_infos *infos);
int					ft_positive_atoi(const char *str);
void				clear_philo(t_philo *philo);
void				time_to_eat(t_philo *philo);
void				time_to_sleep(t_philo *philo);
void				time_to_die(t_philo *philo, int t_time);
void				time_to_start(t_philo *philo);
unsigned long long	find_time(void);

#endif
