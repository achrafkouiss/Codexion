/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:56 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/12 15:28:59 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "pthread.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

// typedef struct s_coder t_coder;
typedef struct s_heap t_heap;
typedef struct s_dongle t_dongle;

typedef struct s_coder
{
	pthread_t		coder_id;
	size_t			id;
	int				counter;
	long			last_compile_time;
	long			priority;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_mutex_t *lock2;
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
	int			status;
	long		last_compile_time;
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


t_input				*parsing(int ac, char *av[]);
void				*routine(void *arg);
void				create_threads(t_coder *coders, size_t capacity);
int					codexion(int ac, char *av[]);


#endif