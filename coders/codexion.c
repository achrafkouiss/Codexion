/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:44 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/04 22:13:38 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
// don't forget to add  max coders
void	*free_dongles(t_dongle *dongles, size_t size)
{
	size_t	i;

	if (!dongles)
		return (NULL);
	i = 0;
	while (i < size)
	{
		if (dongles[i].request)
			free(dongles[i].request->arr);
		pthread_mutex_destroy(&dongles[i].lock);
		pthread_mutex_destroy(&dongles[i].request->lock);
		pthread_cond_destroy(&dongles[i].cond);
		free(dongles[i].request);
		i++;
	}
	free(dongles);
	return (NULL);
}

void	*free_coders(t_coder *coders, t_dongle *dongles, size_t size)
{
	size_t	i;

	if (!dongles)
		return (NULL);
	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(&coders[i].state_lock);
		// pthread_mutex_destroy(&coders[i].lock);

		i++;
	}
	free(coders);
	coders = NULL;
	return (free_dongles(dongles, size));
}

// t_heap	*init_heap(t_dongle *dongles, size_t capacity, size_t size)
// {
// 	t_coder	*coder;
// 	t_heap	*heap;

// 	heap = malloc(sizeof(t_heap));
// 	if (!heap)
// 		return (NULL);
// 	heap->arr = malloc(sizeof(t_coder) * 2);
// 	if (!heap->arr)
// 		return (NULL);
// 	heap->capacity = capacity;
// 	heap->size = 0;
// 	if (pthread_mutex_init(&heap->lock, NULL))
// 			return (free_dongles(dongles, size));
// 	return (heap);
// }

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
		// if (pthread_mutex_init(&coders[i].lock, NULL))
		// 	return (free_coders(coders, dongles, capacity));
		coders[i].last_compile_time = time_in_ms() - inputs->time_to_compile;
		coders[i].id = i;
		coders[i].inputs = inputs;
		// coders[i].burnout = 0;
		coders[i].counter = 0;
		coders[i].right_dongle = &dongles[i];
		coders[i].print_lock = lock;
		if (capacity == 1)
			coders[i].left_dongle = NULL;
		else if (i == capacity - 1)
			coders[i].left_dongle = &dongles[0];
		else
			coders[i].left_dongle = &dongles[i + 1];
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
		return (free(monitor), free_coders(coders, dongles, coders->inputs->number_of_coders));
	monitor->finished_count = 0;
	monitor->coders = coders;
	monitor->dongles = dongles;
	return (monitor);
}

int	codexion(int ac, char *av[])
{
	t_input			*inputs;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	print_lock;
	t_monitor		*monitor;

	inputs = parsing(ac, av);
	if (!inputs)
		return (0);
	dongles = init_dongle(inputs->number_of_coders);
	coders = init_coders(dongles, inputs->number_of_coders, &print_lock,
			inputs);
	monitor = init_monitor(coders, dongles);
	if (!monitor)
		return (free(inputs), 1);
	inputs->monitor = monitor;
	create_threads(coders, inputs->number_of_coders, monitor);
	free_coders(coders, dongles, inputs->number_of_coders);
	pthread_mutex_destroy(&monitor->monitor_lock);

	free(inputs);
	free(monitor);
	return (0);
}

int	main(int ac, char *av[])
{
	size_t	i;
	t_input	*t_input;

	i = 1;
	codexion(ac, av);
}


// i shoul not forget to see why the result make no sense in this case /codexion 1 800 200 200 200 5 0 fifo the coder burnout in 601 0 burned out wich should be 800 not 601 or 600