/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:56 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/24 17:42:08 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "pthread.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
// don't forget to add a mutex to the ft_print so it is protected from data race/race condition
// typedef struct s_coder t_coder;
typedef struct s_heap t_heap;
typedef struct s_input t_input;
typedef struct s_dongle t_dongle;


typedef struct s_coder
{
	pthread_t		coder_id;
	size_t			id;
	int				counter;
	long long		last_compile_time;
	long long		priority;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_mutex_t *print_lock;
	t_input			*inputs;
}					t_coder;

typedef struct s_heap
{
	t_coder			*arr;
	long			capacity;
	long			size;
}					t_heap;

typedef struct s_dongle
{
	long	dongle_id;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int			status;
	long long	last_compile_time;
	long long		cooldown;
	t_heap			*request;
	
}					t_dongle;

typedef struct s_input
{
	size_t			number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			number_of_compiles_required;
	long			dongle_cooldown;
	char			*scheduler;
	long			start;
}					t_input;

typedef struct s_monitor
{
	pthread_t		monitor_id;
	int				burnout_flag;
}					t_monitor;


// typedef struct s_heap
// {
// 	t_coder			*coder;
// 	long			priority;
// 	t_coder			*next_node;
// }					t_heap;


long long			time_in_ms();
t_input				*parsing(int ac, char *av[]);
void				*routine(void *arg);
void				create_threads(t_coder *coders, size_t capacity);
int					codexion(int ac, char *av[]);

#endif