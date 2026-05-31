/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 16:06:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/26 17:05:54 by akouiss          ###   ########.fr       */
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
    
//     while (coder->left_dongle->dongle_id == dongle->dongle_id &&
//          coder->right_dongle->request->arr[0].id != coder->id)
//     {
//         printf("xxxxxxxxxxxxxxx achraf xxxxxxxxxxxxxxxxx\n");
//         pthread_cond_wait(&dongle->cond, &dongle->lock);
//     }
//     push_coder(coder, dongle->request);
//     while (coder->right_dongle->request->capacity == coder->right_dongle->request->size)
//     {
//         pthread_cond_wait(&dongle->cond, &dongle->lock);
//     }
//     pthread_mutex_unlock(&dongle->lock);
// }

void    *request_dongle(t_coder *coder, t_dongle *dongle)
{
    t_dongle   *smallest;

    // printf("%ld\n", dongle->dongle_id);
    // printf("coder->id = %ld || dongle->dongle_id = %ld\n", coder->id, dongle->dongle_id);
    if (coder->left_dongle->dongle_id
        < coder->right_dongle->dongle_id)
        smallest = coder->left_dongle;
    else
        smallest = coder->right_dongle;

    
    /*
    if this is the SECOND dongle request,
    wait until we become first in the
    smallest dongle queue
    */
    if (dongle != smallest)
    {
        // printf("xxxxxxxxxxxxxxxx achraf xxxxxxxxxxxxxxxxxx\n");
        pthread_mutex_lock(&smallest->lock);

        while (
            smallest->request->size > 0
            && smallest->request->arr[0].id != coder->id
        )
        {
            // printf("xxxxxxxxxxxxxxxx achraf xxxxxxxxxxxxxxxxxx\n");
            pthread_cond_wait(
                &smallest->cond,
                &smallest->lock
            );
            // printf("xxxxxxxxxxxxxxxx kouiss xxxxxxxxxxxxxxxxxx\n");
        }

        pthread_mutex_unlock(&smallest->lock);
    }
    // printf("coder->id = %ld || dongle->dongle_id = %ld\n", coder->id, dongle->dongle_id);
    // printf("%ld\n", dongle->dongle_id);
    pthread_mutex_lock(&dongle->lock);
    coder->priority = time_in_ms();
    push_coder(coder, dongle->request);
    pthread_mutex_unlock(&dongle->lock);

    return (NULL);
}
void check_dongles(t_coder *coder)
{
    if (coder->right_dongle->dongle_id < coder->left_dongle->dongle_id)
    {
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        request_dongle(coder, coder->right_dongle);
        request_dongle(coder, coder->left_dongle);
    }
    else
    {
        // printf("zzzzzzzzzzzzzzzzzzzzzzzzzzz\n");
        request_dongle(coder, coder->left_dongle);
        request_dongle(coder, coder->right_dongle);
    }
}

void ft_print(char *str, t_coder *coder)
{
    long long current_time;

    pthread_mutex_lock(coder->print_lock);
    current_time = time_in_ms() - coder->inputs->start;
    printf("%lld %ld %s\n", current_time , coder->id, str);
    pthread_mutex_unlock(coder->print_lock);
}

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


// void take_dongle(t_coder *coder, t_dongle *dongle)
// {
//     long long  compiled;

//     pthread_mutex_lock(&dongle->lock);

//     compiled = dongle->last_compile_time
//         + coder->inputs->dongle_cooldown;

//     while (
//         dongle->status == 1
//         || compiled > time_in_ms()
//         || dongle->request->arr[0].id != coder->id
//     )
//     {
//         pthread_cond_wait(&dongle->cond, &dongle->lock);

//         compiled = dongle->last_compile_time
//             + coder->inputs->dongle_cooldown;
//     }

//     pop_coder(dongle->request);

//     dongle->status = 1;

//     ft_print("has taken a dongle", coder);

//     pthread_mutex_unlock(&dongle->lock);
// }

// void take_dongle(t_coder *coder, t_dongle *dongle)
// {
//     long long compiled;

//     compiled = dongle->last_compile_time + coder->inputs->dongle_cooldown;

//     while (compiled < time_in_ms() && dongle->status)
//     {
//         pthread_cond_wait(&dongle->cond, &dongle->lock);
//         // printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"); 
//         usleep(coder->inputs->dongle_cooldown * 1000);
//         compiled = dongle->last_compile_time + coder->inputs->dongle_cooldown;
//     }
//     pop_coder(dongle->request);
//     dongle->status = 1;
//     if (dongle->request->capacity != dongle->request->size)
//     pthread_cond_broadcast(&dongle->cond);
//     ft_print("has taken a dongle", coder);
//     // if (dongle->dongle_id == coder->left_dongle->dongle_id)
//     // {
//     //     printf("dongle->dongle_id == coder->left_dongle->dongle_id || %ld == %ld\n", dongle->dongle_id ,coder->left_dongle->dongle_id);
//     //     write(1, "left\n", 5);
//     // }
//     // else{
//     //     printf("dongle->dongle_id == coder->right_dongle->dongle_id || %ld == %ld\n", dongle->dongle_id ,coder->right_dongle->dongle_id);
//     //     write(1, "right\n", 6);
//     // }
    
// }

void release_dongle(t_coder *coder)
{
    pthread_mutex_lock(&coder->left_dongle->lock);
    pthread_mutex_lock(&coder->right_dongle->lock);

    coder->left_dongle->status = 0;
    coder->right_dongle->status = 0;

    pthread_cond_broadcast(&coder->left_dongle->cond);
    pthread_cond_broadcast(&coder->right_dongle->cond);

    pthread_mutex_unlock(&coder->right_dongle->lock);
    pthread_mutex_unlock(&coder->left_dongle->lock);
}

// void release_dongle(t_coder *coder)
// {
//     coder->left_dongle->status = 0;
//     coder->right_dongle->status = 0;
//     // printf("xxxxxxxxxxxxxxxxxxxxxxx achraf xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
//     pthread_cond_broadcast(&coder->left_dongle->cond);
// }
void take_both_dongles(t_coder *coder)
{
    t_dongle   *first;
    t_dongle   *second;

    long long  first_ready;
    long long  second_ready;

    if (coder->left_dongle->dongle_id
        < coder->right_dongle->dongle_id)
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
    // printf("coder->id == %ld\n", coder->id);
    while (1)
    {
        pthread_mutex_lock(&first->lock);
        pthread_mutex_lock(&second->lock);

        first_ready =
            first->last_compile_time
            + coder->inputs->dongle_cooldown;

        second_ready =
            second->last_compile_time
            + coder->inputs->dongle_cooldown;

        if (
            first->status == 0
            && second->status == 0
            && first_ready <= time_in_ms()
            && second_ready <= time_in_ms()
            && first->request->size > 0
            && second->request->size > 0
            && first->request->arr[0].id == coder->id
            && second->request->arr[0].id == coder->id
        )
        {
            pop_coder(first->request);
            pop_coder(second->request);

            first->status = 1;
            second->status = 1;

            ft_print("has taken a dongle", coder);
            ft_print("has taken a dongle", coder);
            // pthread_cond_broadcast(&second->cond);
            pthread_mutex_unlock(&second->lock);
            pthread_mutex_unlock(&first->lock);
            
            return;
        }

        pthread_mutex_unlock(&second->lock);

        // pthread_cond_wait(&first->cond, &first->lock);
        // printf("coder->id = %ld\n", coder->id);
        // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

        pthread_mutex_unlock(&first->lock);
    }
}
void *routine(void *arg)
{
    t_coder *coder;
    
    coder = arg;
    // printf("coder->id = %ld || right_dongle->dongle_id = %ld || left_donle->dongle_id = %ld\n", coder->id, coder->right_dongle->dongle_id, coder->left_dongle->dongle_id);
    coder->last_compile_time = time_in_ms();

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
    // take_dongle(coder, coder->right_dongle);
    // // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    // take_dongle(coder, coder->left_dongle);
    take_both_dongles(coder);
    compiling(coder);
    release_dongle(coder);
    // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
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
    // // printf("coder->id = %ld || left_dongle->dongle_id = %ld\n", coder->id, coder->left_dongle->dongle_id);
    // // printf("coder->id = %ld\n", coder->id);
    // // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    debugging(coder);
    refactoring(coder);
    return NULL;
}

// int main()
// {
//     return (0);
// }
