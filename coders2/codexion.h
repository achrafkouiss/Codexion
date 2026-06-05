/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:56 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/05 12:09:14 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define MAX_CODERS 300

# include "pthread.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_heap	t_heap;
typedef struct s_input	t_input;
typedef struct s_dongle	t_dongle;

typedef struct s_coder
{
	pthread_t			coder_id;
	size_t				id;
	long long			last_compile_time;
	long long			priority;
	int					counter;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	pthread_mutex_t		*print_lock;
	pthread_mutex_t		state_lock;
	t_input				*inputs;
}						t_coder;

typedef struct s_heap_entry
{
	size_t				id;
	long				priority;
}						t_heap_entry;

typedef struct s_heap
{
	t_heap_entry		*arr;
	long				capacity;
	long				size;
	pthread_mutex_t		lock;
}						t_heap;

typedef struct s_dongle
{
	long				dongle_id;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
	int					status;
	long long			last_compile_time;
	t_heap				*request;
}						t_dongle;

typedef struct s_monitor
{
	pthread_t			monitor_id;
	pthread_mutex_t		monitor_lock;
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
int						is_stopped(t_input *inputs);

// free_resources.c
void					*free_dongles(t_dongle *dongles, size_t size);
void					*free_coders(t_coder *coders, t_dongle *dongles,
							size_t size);

// minheap_implementation.c
void					push_coder(size_t id, long priority, t_heap *heap);
size_t					pop_coder(t_heap *heap);

// process.c
void					lock_unlock_last_compile_time(t_coder *c,
							t_dongle *dongle);
void					compiling(t_coder *coder);
void					debugging(t_coder *coder);
void					refactoring(t_coder *coder);

// dongle.c
void					get_ordered_dongles(t_coder *coder,
							t_dongle **first, t_dongle **second);
int						take_both_dongles(t_coder *coder);
void					release_dongle(t_coder *coder);

// request.c
void					*request_dongle(t_coder *coder, t_dongle *dongle);
void					check_dongles(t_coder *coder);

t_input					*parsing(int ac, char *av[]);
void					*routine(void *arg);
void					*monitor_routine(void *arg);
void					create_threads(t_coder *coders, size_t capacity,
							t_monitor *monitor);
int						codexion(int ac, char *av[]);

// initialization.c
t_heap					*init_heap(t_dongle *dongles, size_t capacity,
							size_t size);
t_dongle				*init_dongle(size_t capacity);
t_coder					*init_coders(t_dongle *dongles, size_t capacity,
							pthread_mutex_t *lock, t_input *inputs);
t_monitor				*init_monitor(t_coder *coders, t_dongle *dongles);

#endif
