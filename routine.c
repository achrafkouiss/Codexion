/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 16:06:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/22 11:57:59 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long time_in_ms()
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0) {
        fprintf(stderr, "gettimeofday failed");
        return (-1);
    }
    return ((long long)tv.tv_sec * 1000) + ((long long)tv.tv_usec / 1000);
}

void swap(t_coder *a, t_coder *b)
{
    t_coder temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void push_coder(t_coder *coder, t_heap *heap)
{
    size_t index;

    index = heap->size;
    heap->arr[heap->size++] = *coder;
    while (index > 0 && heap->arr[(index - 1) / 2].priority > heap->arr[index].priority)
    {
        swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}
void heap_loop(t_heap  *heap)
{
    size_t l;
    size_t r;
    size_t i;
    size_t  smallest;

    i = 0;
    while (1)
    {
        smallest = i;
        l = 2 * i + 1;
        r = 2 * i + 2;
        if (l < heap->size && heap->arr[l].priority
            < heap->arr[smallest].priority)
            smallest = l;
        if (r < heap->size && heap->arr[r].priority
            < heap->arr[smallest].priority)
            smallest = r;
        if (smallest == i)
            break;
        swap(&heap->arr[i], &heap->arr[smallest]);
        i = smallest;
    }
}

t_coder pop_coder(t_heap  *heap)
{
    t_coder popped;
  
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

// void *request_dongle(t_coder *coder, t_dongle *dongle)
// {
//     pthread_mutex_lock(&dongle->lock);
//     coder->priority = time_in_ms();
//     while (dongle == coder->left_dongle && coder->left_dongle->request->size)
//     pthread_cond_wait(&dongle->cond, &dongle->lock);
//     push_coder(coder, dongle->request);
//     while (coder->left_dongle == dongle && dongle->status)
//     pthread_cond_broadcast(&dongle->cond);
//     pthread_mutex_unlock(&dongle->lock);
// }

void *request_dongle(t_coder *coder, t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->lock);
    coder->priority = time_in_ms();
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  achraf xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    while (dongle == coder->left_dongle && coder->left_dongle->request->size)
    pthread_cond_wait(&dongle->cond, &dongle->lock);
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  kouiss xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    push_coder(coder, dongle->request);
    printf("dongle->request->capacity = %ld || dongle->request->size = %ld\n", dongle->request->capacity, dongle->request->size);
    // while (coder->left_dongle == dongle && dongle->status)
    while (dongle->request->capacity == dongle->request->size)
    pthread_cond_wait(&dongle->cond, &dongle->lock);
    // pthread_cond_broadcast(&dongle->cond);
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  khalid xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    pthread_mutex_unlock(&dongle->lock);
}

void check_dongles(t_coder *coder)
{
    if (coder->right_dongle->dongle_id < coder->left_dongle->dongle_id)
    {
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx coder->id = %ld\n", coder->id);
        request_dongle(coder, coder->right_dongle);
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx coder->id = %ld\n", coder->id);
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        request_dongle(coder, coder->left_dongle);
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx coder->id = %ld\n", coder->id);
    }
    else
    {
        request_dongle(coder, coder->left_dongle);
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        request_dongle(coder, coder->right_dongle);
    }
}

void take_dongle(t_coder *coder, t_dongle *dongle)
{
    long long compiled;

    compiled = time_in_ms() - coder->last_compile_time;
    printf("coder->id = %ld has taken the dungles\n", coder->id);
    while (compiled < coder->inputs->dongle_cooldown && dongle->status)
    {
        // printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
        usleep(5000);
    }
    pop_coder(coder->left_dongle->request);
    pop_coder(coder->right_dongle->request);
    dongle->status = 1;
    if (dongle->request->capacity != dongle->request->size)
    {
        // printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");   
        pthread_cond_broadcast(&dongle->cond);
    }
}

void compiling(t_coder *coder)
{
    usleep(coder->inputs->time_to_compile);
}

void *routine(void *arg)
{
    t_coder *coder;
    
    coder = arg;
    printf("coder->id = %ld || right_dongle->dongle_id = %ld || left_donle->dongle_id = %ld\n", coder->id, coder->right_dongle->dongle_id, coder->left_dongle->dongle_id);
    // gettimeofday(&coder->last_compile_time, NULL);
    coder->last_compile_time = time_in_ms();
    check_dongles(coder);
    // request_dongle(coder, coder->right_dongle);
    // printf("coder->id = %ld || right_dongle->dongle_id = %ld\n", coder->id, coder->right_dongle->dongle_id);
    // request_dongle(coder, coder->left_dongle);
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    pthread_mutex_lock(coder->lock2);
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    printf("coder->coder_id = %ld\n", coder->coder_id);
    printf("coder->coder_id = %ld\n", coder->id);
    printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
    for (int i = 0; i < 2; i++)
    {
        printf("right_dongle == %ld\n", coder->right_dongle->request->arr[i].id);
        // printf("%ld\n", coder->left_dongle->request->arr[i].coder_id);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
    for (int i = 0; i < 2; i++)
    {
        printf("lef_dongle == %ld\n", coder->left_dongle->request->arr[i].id);
        // printf("%ld\n", coder->left_dongle->request->arr[i].coder_id);
    }
    pthread_mutex_unlock(coder->lock2);
    // printf("coder->id = %ld || left_dongle->dongle_id = %ld\n", coder->id, coder->left_dongle->dongle_id);
    // printf("coder->id = %ld\n", coder->id);
    take_dongle(coder, coder->right_dongle);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    take_dongle(coder, coder->left_dongle);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    
    return NULL;
}

// int main()
// {
//     return (0);
// }
