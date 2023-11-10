/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 12:57:27 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/10 15:58:35 by vst-pier         ###   ########.fr       */
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

typedef struct s_infos
{
	int			number_of_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			must_eat;
	int			dead_philo;
	long long	time_of_death;
	long long	start_time;
}	t_infos;

typedef struct s_philo
{
	int				no;
	int				philo_state;
	pthread_t		pt_philo;
	pthread_mutex_t	fork;
	int				fork_state;
	int				finished;
	int				meal_eaten;
	struct s_infos	*infos;
	struct s_philo	*right_philo;
	int				my_turn;
	int				thinking;
	long long		last_meal;
	pthread_mutex_t	start;
	pthread_mutex_t	end;
}	t_philo;

int			check_is_number(char **argv);
void		check_death(t_philo *philo);
t_infos		*initialize_infos(char **argv);
t_philo		*initialize_philo(t_infos *infos, int no);
t_philo		*create_philo(t_infos *infos);
int			ft_positive_atoi(const char *str);
void		clear_philo(t_philo *philo);
void		time_to_think(t_philo *philo);
void		time_to_eat(t_philo *philo);
void		time_to_sleep(t_philo *philo);
void		time_to_die(t_philo *philo);
void		time_to_start(t_philo *philo);
long long	find_time(void);

#endif
