/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:44 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/05 12:03:20 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap	*init_heap(t_dongle *dongles, size_t capacity, size_t size)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->arr = malloc(sizeof(t_heap_entry) * capacity);
	if (!heap->arr)
		return (NULL);
	heap->capacity = capacity;
	heap->size = 0;
	if (pthread_mutex_init(&heap->lock, NULL))
		return (free_dongles(dongles, size));
	return (heap);
}

t_dongle	*init_dongle(size_t capacity)
{
	size_t		i;
	t_dongle	*dongles;

	i = 0;
	dongles = malloc(capacity * sizeof(t_dongle));
	if (!dongles)
		return (NULL);
	while (i < capacity)
	{
		if (pthread_mutex_init(&dongles[i].lock, NULL)
			|| pthread_cond_init(&dongles[i].cond, NULL))
			return (free_dongles(dongles, i));
		dongles[i].dongle_id = i;
		dongles[i].status = 0;
		dongles[i].last_compile_time = 0;
		dongles[i].request = init_heap(dongles, capacity, i);
		if (!dongles[i].request)
			return (free_dongles(dongles, i));
		i++;
	}
	return (dongles);
}

static void	init_coder_entry(t_coder *coders, t_dongle *dongles, size_t i,
		t_input *inputs)
{
	coders[i].last_compile_time = time_in_ms() - inputs->time_to_compile;
	coders[i].id = i;
	coders[i].inputs = inputs;
	coders[i].counter = 0;
	coders[i].right_dongle = &dongles[i];
	if (inputs->number_of_coders == 1)
		coders[i].left_dongle = NULL;
	else if (i == inputs->number_of_coders - 1)
		coders[i].left_dongle = &dongles[0];
	else
		coders[i].left_dongle = &dongles[i + 1];
}

t_coder	*init_coders(t_dongle *dongles, size_t capacity, pthread_mutex_t *lock,
		t_input *inputs)
{
	size_t	i;
	t_coder	*coders;

	if (!dongles)
		return (NULL);
	if (pthread_mutex_init(lock, NULL))
		return (free_dongles(dongles, capacity));
	i = 0;
	coders = malloc(capacity * sizeof(t_coder));
	if (!coders)
	{
		pthread_mutex_destroy(lock);
		return (free_dongles(dongles, capacity));
	}
	while (i < capacity)
	{
		if (pthread_mutex_init(&coders[i].state_lock, NULL))
			return (free_coders(coders, dongles, capacity));
		init_coder_entry(coders, dongles, i, inputs);
		coders[i].print_lock = lock;
		i++;
	}
	return (coders);
}

t_monitor	*init_monitor(t_coder *coders, t_dongle *dongles)
{
	t_monitor	*monitor;

	if (!coders || !dongles)
		return (NULL);
	monitor = malloc(sizeof(t_monitor));
	if (!monitor)
		return (free_coders(coders, dongles, coders->inputs->number_of_coders));
	if (pthread_mutex_init(&monitor->monitor_lock, NULL))
		return (free(monitor), free_coders(coders, dongles,
				coders->inputs->number_of_coders));
	monitor->finished_count = 0;
	monitor->coders = coders;
	monitor->dongles = dongles;
	return (monitor);
}
