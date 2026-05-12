/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:44 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/12 15:57:22 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void *free_dongles(t_dongle *dongles, size_t size)
{
	size_t i;

	i = 0;
	while (i < size)
	{
		free(dongles[i].request->arr);
		free(dongles[i].request);
		i++;
	}
	free(dongles);
	dongles = NULL;
	return (NULL);
}

void *free_coders(t_coder *coders, t_dongle *dongles, size_t size)
{
	// printf("xxxxxxxxxxxxxxxxxx\n");
	free(coders);
	coders = NULL;
	return (free_dongles(dongles, size));
}

t_heap *init_heap(t_dongle *dongles, size_t capacity, size_t size)
{
	t_coder *coder;
	t_heap *heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (free_dongles(dongles, size));
	heap->arr = malloc(sizeof(t_coder) * 2);
	if (!heap->arr)
		return (free_dongles(dongles, size));
	heap->capacity = capacity;
	heap->size = 0;
	return (heap);
}

t_dongle *init_dongle(size_t capacity)
{
	size_t i;
	t_dongle *dongles;

	i = 0;
	dongles = malloc(capacity * sizeof(t_dongle));
	if (!dongles)
		return free_dongles(dongles, i);
	while (i < capacity)
	{
		pthread_mutex_init(&dongles[i].lock, NULL);
		dongles[i].dongle_id = i;
		dongles[i].status = 0;
		dongles[i].last_compile_time = 0;
		dongles[i].request = init_heap(dongles, capacity, i);
		i++;
	}
	return dongles;
}

t_coder *init_coders(t_dongle *dongles, size_t capacity, pthread_mutex_t *lock)
{
	size_t i;
	t_coder		*coders;

	if (!dongles)
		return (NULL);
	i = 0;
	coders = malloc(capacity * sizeof(t_coder));
	if (!coders)
		return (free_coders(coders, dongles, capacity));
	while (i < capacity)
	{
		coders[i].counter = 0;
		coders[i].last_compile_time = 0;
		coders[i].priority = 0;
		coders[i].id = i;
		coders[i].right_dongle = &dongles[i];
		coders[i].lock2 = lock;
		if (capacity == 1)
		coders[i].left_dongle = NULL;
		else if (i == capacity - 1)
		coders[i].left_dongle = &dongles[0];
		else
		coders[i].left_dongle = &dongles[i + 1];
		i++;
		// printf("xxxxxxxxxxxxxxxxxx\n");
	}
	return (coders);
}

int	codexion(int ac, char *av[])
{
	t_input		*inputs;
	t_dongle	*dongles;
	t_coder		*coders;
	pthread_mutex_t lock;
	// t_monitor	*monitor;

	inputs = parsing(ac, av);
	if (!inputs)
		return (0);
	// printf("size = %ld\n", inputs->number_of_coders);
	dongles = init_dongle(inputs->number_of_coders);
	coders = init_coders(dongles, inputs->number_of_coders, &lock);
	if (!coders)
	return  (free(inputs), 0);
	// printf("size = %ld\n", inputs->number_of_coders);
	create_threads(coders, inputs->number_of_coders);
	// printf("xxxxxxxxxxxxxxxxxx\n");
	// for (int i = 0; i < inputs->number_of_coders; i++)
	// {
		// 	printf("xxxxxxxxxxxxxxxxxxx\n");
		// 	printf("i = %d\n", i);
		// 	printf("coders[%d] = %ld\n", i, coders[i]->coder_id);
		// 	printf("left = %ld\n", coders[i]->left_dongle->dongle_id);
		// 	printf("right = %ld\n", coders[i]->right_dongle->dongle_id);
		// }
	free_coders(coders, dongles, inputs->number_of_coders);
	free(inputs);
	return (0);
}

int	main(int ac, char *av[])
{
	size_t		i;
	t_input	*t_input;

	i = 1;
	codexion(ac, av);
}
