/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:56 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/31 23:43:19 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "pthread.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
// don't forget to add a mutex to the ft_print so it is protected from data race/race condition
// typedef struct s_coder t_coder;
typedef struct s_heap	t_heap;
typedef struct s_input	t_input;
typedef struct s_dongle	t_dongle;

typedef struct s_coder
{
	pthread_t			coder_id;
	size_t				id;
	int					counter;
	long long			last_compile_time;
	long long			priority;
	int					burnout;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	pthread_mutex_t		*print_lock;
	pthread_mutex_t		state_lock;
	t_input				*inputs;
}						t_coder;

typedef struct s_heap
{
	t_coder				*arr;
	long				capacity;
	long				size;
}						t_heap;

typedef struct s_dongle
{
	long				dongle_id;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
	int					status;
	long long			last_compile_time;
	long long			cooldown;
	t_heap				*request;

}						t_dongle;

typedef struct s_monitor
{
	pthread_t			monitor_id;
	pthread_mutex_t		monitor_lock;
	// pthread_mutex_t		monitor_cond;
	// int				burnout_flag;
	int					finished_count;
	t_coder				*coders;
	t_dongle			*dongles;
}						t_monitor;

typedef struct s_input
{
	size_t				number_of_coders;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	long				number_of_compiles_required;
	long				dongle_cooldown;
	char				*scheduler;
	long				start;

	int					stop;
	t_monitor			*monitor;
	pthread_mutex_t		stop_lock;

}						t_input;

// helper.c
long long				time_in_ms(void);
void					timed_sleep(t_coder *coder, long long time);
void					ft_print(char *str, t_coder *coder, int force);
void					burnout_check(t_coder *coder);
int						is_stopped(t_input *inputs);

// minheap_implementation.c
void					push_coder(t_coder *coder, t_heap *heap);
t_coder					pop_coder(t_heap *heap);

// process.c
void					compiling(t_coder *coder);
void					debugging(t_coder *coder);
void					refactoring(t_coder *coder);

t_input					*parsing(int ac, char *av[]);
void					*routine(void *arg);
void					*monitor_routine(void *arg);
void					create_threads(t_coder *coders, size_t capacity,
							t_monitor *monitor);
int						codexion(int ac, char *av[]);

#endif