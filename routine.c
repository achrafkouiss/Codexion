/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 16:06:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/24 17:59:21 by akouiss          ###   ########.fr       */
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
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  achraf xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    while (dongle == coder->left_dongle && coder->left_dongle->request->size)
    pthread_cond_wait(&dongle->cond, &dongle->lock);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  kouiss xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    push_coder(coder, dongle->request);
    // printf("dongle->request->capacity = %ld || dongle->request->size = %ld\n", dongle->request->capacity, dongle->request->size);
    // while (coder->left_dongle == dongle && dongle->status)
    while (dongle->request->capacity == dongle->request->size)
    pthread_cond_wait(&dongle->cond, &dongle->lock);
    // pthread_cond_broadcast(&dongle->cond);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  khalid xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    pthread_mutex_unlock(&dongle->lock);
}

void check_dongles(t_coder *coder)
{
    if (coder->right_dongle->dongle_id < coder->left_dongle->dongle_id)
    {
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx coder->id = %ld\n", coder->id);
        request_dongle(coder, coder->right_dongle);
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx coder->id = %ld\n", coder->id);
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        request_dongle(coder, coder->left_dongle);
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx coder->id = %ld\n", coder->id);
    }
    else
    {
        request_dongle(coder, coder->left_dongle);
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        request_dongle(coder, coder->right_dongle);
    }
}

void ft_print(char *str, t_coder *coder)
{
    // pthread_mutex_t lock;
    long long current_time;

    // pthread_mutex_init(&lock, NUeLL);
    pthread_mutex_lock(coder->print_lock);
    current_time = time_in_ms() - coder->inputs->start;
    // printf("current_time = time_in_ms() - coder->inputs->start || %lld = %lld - %ld\n", current_time, time_in_ms(), coder->inputs->start);
    printf("%lld %ld %s\n", current_time , coder->id, str);
    pthread_mutex_unlock(coder->print_lock);
    // pthread_mutex_destroy(&lock);
}

// void ft_fprint(char *str, t_coder *coder, t_dongle *dongle)
// {
//     pthread_mutex_t lock;
//     long long current_time;

//     pthread_mutex_init(&lock, NULL);
//     pthread_mutex_lock(&lock);
//     current_time = time_in_ms() - coder->inputs->start;
//     fprintf(STDERR_FILENO, "%lld %ld %s", current_time , coder->id, str);
//     pthread_mutex_unlock(&lock);
//     pthread_mutex_destroy(&lock);
// }

void compiling(t_coder *coder)
{
    ft_print("is compiling", coder);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    coder->last_compile_time = time_in_ms();
    usleep(coder->inputs->time_to_compile * 1000);
    // printf()
    coder->left_dongle->last_compile_time = time_in_ms();
    coder->right_dongle->last_compile_time = time_in_ms();
}

void debugging(t_coder *coder)
{
    ft_print("is debugging", coder);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    usleep(coder->inputs->time_to_debug * 1000);
}

void refactoring(t_coder *coder)
{
    ft_print("is refactoring", coder);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    usleep(coder->inputs->time_to_refactor * 1000);
}

void take_dongle(t_coder *coder, t_dongle *dongle)
{
    long long compiled;

    compiled = dongle->last_compile_time + coder->inputs->dongle_cooldown;
    // printf("compiled = dongle->last_compile_time + coder->inputs->dongle_cooldown ||  %lld = %lld - %ld\n", compiled, dongle->last_compile_time, coder->inputs->dongle_cooldown);
    // printf("coder->id = %ld has taken the dungles\n", coder->id);
    // printf("compiled < coder->inputs->dongle_cooldown = %lld < %ld\n", compiled, coder->inputs->dongle_cooldown);
    while (compiled < time_in_ms() && dongle->status)
    {
        pthread_cond_wait(&dongle->cond, &dongle->lock);
        // printf("compiled <  time_in_ms() = %lld < %lld\n", compiled,  time_in_ms());
        // printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"); 
        // printf("dongle->status = %d\n", dongle->status);
        usleep(coder->inputs->dongle_cooldown * 1000);
        compiled = dongle->last_compile_time + coder->inputs->dongle_cooldown;
    }
    pop_coder(dongle->request);
    dongle->status = 1;
    ft_print("has taken a dongle", coder);
    if (dongle->request->capacity != dongle->request->size)
        pthread_cond_broadcast(&dongle->cond);
    
}

void release_dongle(t_coder *coder)
{
    coder->left_dongle->status = 0;
    coder->right_dongle->status = 0;
    // usleep(10000);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    // coder->left_dongle->last_compile_time = f
    pthread_cond_broadcast(&coder->left_dongle->cond);
}

void *routine(void *arg)
{
    t_coder *coder;
    
    coder = arg;
    // printf("coder->id = %ld || right_dongle->dongle_id = %ld || left_donle->dongle_id = %ld\n", coder->id, coder->right_dongle->dongle_id, coder->left_dongle->dongle_id);
    // gettimeofday(&coder->last_compile_time, NULL);
    coder->last_compile_time = time_in_ms();
    // coder->last_compile_time = 0;
    // sleep(1);
    check_dongles(coder);
    // request_dongle(coder, coder->right_dongle);
    // printf("coder->id = %ld || right_dongle->dongle_id = %ld\n", coder->id, coder->right_dongle->dongle_id);
    // request_dongle(coder, coder->left_dongle);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    // pthread_mutex_lock(coder->lock2);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    // printf("coder->coder_id = %ld\n", coder->coder_id);
    // printf("coder->coder_id = %ld\n", coder->id);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
    // for (int i = 0; i < 2; i++)
    // {
    //     printf("right_dongle == %ld\n", coder->right_dongle->request->arr[i].id);
    //     // printf("%ld\n", coder->left_dongle->request->arr[i].coder_id);
    // }
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
    // for (int i = 0; i < 2; i++)
    // {
    //     printf("lef_dongle == %ld\n", coder->left_dongle->request->arr[i].id);
    //     // printf("%ld\n", coder->left_dongle->request->arr[i].coder_id);
    // }
    // pthread_mutex_unlock(coder->lock2);
    // // printf("coder->id = %ld || left_dongle->dongle_id = %ld\n", coder->id, coder->left_dongle->dongle_id);
    // // printf("coder->id = %ld\n", coder->id);
    take_dongle(coder, coder->right_dongle);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    take_dongle(coder, coder->left_dongle);
    compiling(coder);
    release_dongle(coder);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    debugging(coder);
    refactoring(coder);
    return NULL;
}

// int main()
// {
//     return (0);
// }
