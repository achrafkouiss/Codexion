/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minheap_implementation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 01:37:38 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/31 01:37:38 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap(t_coder *a, t_coder *b)
{
	t_coder	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static void	heap_loop(t_heap *heap)
{
	size_t	l;
	size_t	r;
	size_t	i;
	size_t	smallest;

	i = 0;
	while (1)
	{
		smallest = i;
		l = 2 * i + 1;
		r = 2 * i + 2;
		if (l < heap->size
			&& heap->arr[l].priority < heap->arr[smallest].priority)
			smallest = l;
		if (r < heap->size
			&& heap->arr[r].priority < heap->arr[smallest].priority)
			smallest = r;
		if (smallest == i)
			break ;
		swap(&heap->arr[i], &heap->arr[smallest]);
		i = smallest;
	}
}

void	push_coder(t_coder *coder, t_heap *heap)
{
	size_t	index;

	index = heap->size;
	heap->arr[heap->size++] = *coder;
	while (index > 0 && heap->arr[(index - 1)
		/ 2].priority > heap->arr[index].priority)
	{
		swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
		index = (index - 1) / 2;
	}
}

t_coder	pop_coder(t_heap *heap)
{
	t_coder	popped;

	if (heap->size == 0)
		return ((t_coder){0});
	popped = heap->arr[0];
	heap->size--;
	if (heap->size == 0)
		return (popped);
	heap->arr[0] = heap->arr[heap->size];
	heap_loop(heap);
	return (popped);
}
