/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 16:06:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/12 16:00:54 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void swap(t_coder *a, t_coder *b)
{
    t_coder temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void push_coder(t_coder *coder, t_dongle *dongle)
{
    t_heap *heap;
    size_t index;
    // t_heap dongle;

    heap = dongle->request;
    index = heap->size;
    heap->arr[heap->size++] = *coder;
    while (index > 0 && heap->arr[(index - 1) / 2].priority > heap->arr[index].priority)
    {
        swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

// void print_message()
// {
//     printf("take")
// }

void *request_dongle(t_coder *coder)
{
    t_dongle *left_dongle;
    t_dongle *right_dongle;
    struct timeval tv;

    left_dongle = coder->left_dongle;
    right_dongle = coder->right_dongle;
    // printf("%ld\n", coder->coder_id);
    // pthread_mutex_lock(&right_dongle->lock);
    coder->priority = gettimeofday(&tv, NULL);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
    if (!right_dongle->status)
    {
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
        right_dongle->status = 1;
        push_coder(coder, right_dongle);
        // printf("%ld\n", coder->right_dongle->request->arr[0].coder_id);
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
    }
    else if (right_dongle->status)
    {
        if (!left_dongle->status)
        {
            right_dongle->status = 1;
            push_coder(coder, left_dongle);
        }
    }
}

void *routine(void *arg)
{
    t_coder *coder;
    
    coder = arg;
    request_dongle(coder);
    request_dongle(coder);
    pthread_mutex_lock(coder->lock2);
    printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
    printf("coder->coder_id = %ld\n", coder->coder_id);
    printf("coder->coder_id = %ld\n", coder->id);
    for (int i = 0; i < 2; i++)
    {
        printf("%ld\n", coder->right_dongle->request->arr[i].coder_id);
        // printf("%ld\n", coder->left_dongle->request->arr[i].coder_id);
    }
    pthread_mutex_unlock(coder->lock2);
    return NULL;
}

// int main()
// {
//     return (0);
// }
