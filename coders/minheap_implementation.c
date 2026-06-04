// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   minheap_implementation.c                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2026/05/31 01:37:38 by akouiss           #+#    #+#             */
// /*   Updated: 2026/06/04 17:33:40 by akouiss          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "codexion.h"

static void swap(t_heap_entry *a, t_heap_entry *b)
{
    t_heap_entry temp = *a;
    *a = *b;
    *b = temp;
}

static void heap_loop(t_heap *heap)
{
    size_t i = 0;
    while (1)
    {
        size_t smallest = i;
        size_t l = 2 * i + 1;
        size_t r = 2 * i + 2;
        if (l < (size_t)heap->size && heap->arr[l].priority < heap->arr[smallest].priority)
            smallest = l;
        if (r < (size_t)heap->size && heap->arr[r].priority < heap->arr[smallest].priority)
            smallest = r;
        if (smallest == i)
            break;
        swap(&heap->arr[i], &heap->arr[smallest]);
        i = smallest;
    }
}

void push_coder(size_t id, long priority, t_heap *heap)
{
    size_t index = heap->size;
    heap->arr[index].id = id;
    heap->arr[index].priority = priority;
    heap->size++;
    while (index > 0 && heap->arr[(index - 1) / 2].priority > heap->arr[index].priority)
    {
        swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

size_t pop_coder(t_heap *heap)
{
    if (heap->size == 0)
        return ((size_t)-1);
    size_t id = heap->arr[0].id;
    heap->size--;
    if (heap->size == 0)
        return (id);
    heap->arr[0] = heap->arr[heap->size];
    heap_loop(heap);
    return (id);
}