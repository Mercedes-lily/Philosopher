/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vst-pier <vst-pier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 12:57:27 by vst-pier          #+#    #+#             */
/*   Updated: 2023/11/29 13:57:02 by vst-pier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <limits.h>
# include <sys/time.h>
# include <stdlib.h>

typedef unsigned long long	t_ull;

typedef struct s_god
{
	int					first_death_philo;
	t_ull				time_of_death;
	int					finished;
	pthread_mutex_t		end;
	int					dead;
	pthread_t			pt_god;
	pthread_mutex_t		printf;
}	t_god;

typedef struct s_infos
{
	int					number_of_philosophers;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					must_eat;
	pthread_mutex_t		start;
	t_ull				start_time;
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
	struct s_god			*god;
	int						thinking;
	t_ull					last_meal;
	t_ull					time_of_death;
	pthread_mutex_t			fork;
}	t_philo;

int		check_is_number(char **argv);
void	check_death(t_philo *philo);
t_infos	*initialize_infos(char **argv);
t_philo	*initialize_philo(t_infos *infos, t_god *god, int no);
t_god	*initialize_god(t_infos *infos);
t_philo	*create_philo(t_infos *infos, t_god *god);
int		ft_positive_atoi(const char *str);
void	clear_philo(t_philo *philo);
void	time_to_eat(t_philo *philo);
void	time_to_sleep(t_philo *philo);
void	time_to_die(t_philo *philo, int t_time);
void	time_to_start(t_philo *philo);
t_ull	find_time(void);
void	*ft_calloc(size_t count, size_t size);
int		ft_isdigit(int c);
void	state(t_philo *philo, char *s, t_ull current_time);

#endif
