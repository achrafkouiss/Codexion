// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   preious.c                                          :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2026/05/11 16:06:54 by akouiss           #+#    #+#             */
// /*   Updated: 2026/05/20 11:14:38 by akouiss          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "codexion.h"

// long long time_in_ms(struct timeval tv) {
//     if (gettimeofday(&tv, NULL) != 0) {
//         fprintf(stderr, "gettimeofday failed");
//         return (-1);
//     }
//     return ((long long)tv.tv_sec * 1000) + ((long long)tv.tv_usec / 1000);
// }

// void swap(t_coder *a, t_coder *b)
// {
//     t_coder temp;

//     temp = *a;
//     *a = *b;
//     *b = temp;
// }

// void push_coder(t_coder *coder, t_heap *heap)
// {
//     size_t index;

//     index = heap->size;
//     heap->arr[heap->size++] = *coder;
//     while (index > 0 && heap->arr[(index - 1) / 2].priority.tv_sec > heap->arr[index].priority.tv_sec)
//     {
//         swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
//         index = (index - 1) / 2;
//     }
// }
// void heap_loop(t_heap  *heap)
// {
//     size_t l;
//     size_t r;
//     size_t i;
//     size_t  smallest;

//     i = 0;
//     while (1)
//     {
//         smallest = i;
//         l = 2 * i + 1;
//         r = 2 * i + 2;
//         if (l < heap->size && heap->arr[l].priority.tv_sec
//             < heap->arr[smallest].priority.tv_sec)
//             smallest = l;
//         if (r < heap->size && heap->arr[r].priority.tv_sec
//             < heap->arr[smallest].priority.tv_sec)
//             smallest = r;
//         if (smallest == i)
//             break;
//         swap(&heap->arr[i], &heap->arr[smallest]);
//         i = smallest;
//     }
// }

// t_coder pop_coder(t_heap  *heap)
// {
//     t_coder popped;
  
//     if (heap->size == 0)
//         return ((t_coder){0});
//     popped = heap->arr[0];
//     heap->size--;
//     if (heap->size == 0)
//         return (popped);
//     heap->arr[0] = heap->arr[heap->size];
//     heap_loop(heap);
//     return (popped);
// }

// void *request_dongle(t_coder *coder, t_dongle *dongle)
// {
//     t_dongle *left_dongle;
//     t_dongle *right_dongle;

//     right_dongle = coder->right_dongle;
//     // printf("%ld\n", coder->coder_id);
//     pthread_mutex_lock(&right_dongle->lock);
//     gettimeofday(&coder->priority, NULL);
//     // printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
//     push_coder(coder, right_dongle);
//     pthread_mutex_unlock(&right_dongle->lock);
// }

// // void *request_left_dongle(t_coder *coder)
// // {
// //     t_dongle *left_dongle;
// //     t_dongle *right_dongle;

// //     left_dongle = coder->left_dongle;
// //     right_dongle = coder->right_dongle;
// //     // printf("%ld\n", coder->coder_id);
// //     pthread_mutex_lock(&left_dongle->lock);
// //     gettimeofday(&coder->priority, NULL);
// //     printf("======> coder->coder_id = %ld\n", coder->id);
// //     printf("======> coder->priority = %ld\n", coder->priority.tv_usec);
// //     printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
// //     while (left_dongle->request->size)
// //     pthread_cond_wait(&left_dongle->cond, &left_dongle->lock);
// //     push_coder(coder, left_dongle);
// //     pthread_mutex_unlock(&left_dongle->lock);
// // }

// void take_dongle(t_coder *coder)
// {
//     while ()
//     pop_coder(coder->right_dongle);
// }

// void *routine(void *arg)
// {
//     t_coder *coder;
    
//     coder = arg;
//     gettimeofday(&coder->last_compile_time, NULL);
//     request_dongle(coder, coder->right_dongle);
//     request_dongle(coder, coder->left_dongle);
//     // pthread_mutex_lock(coder->lock2);
//     printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
//     printf("coder->coder_id = %ld\n", coder->coder_id);
//     printf("coder->coder_id = %ld\n", coder->id);
//     printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
//     for (int i = 0; i < 2; i++)
//     {
//         printf("right_dongle == %ld\n", coder->right_dongle->request->arr[i].coder_id);
//         // printf("%ld\n", coder->left_dongle->request->arr[i].coder_id);
//     }
//     printf("xxxxxxxxxxxxxxxxxxxxxxxxx\n");
//     for (int i = 0; i < 2; i++)
//     {
//         printf("lef_dongle == %ld\n", coder->left_dongle->request->arr[i].coder_id);
//         // printf("%ld\n", coder->left_dongle->request->arr[i].coder_id);
//     }
//     // pthread_mutex_unlock(coder->lock2);
//     return NULL;
// }

// // int main()
// // {
// //     return (0);
// // }



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 16:06:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/31 01:51:30 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"



void    *request_dongle(t_coder *coder, t_dongle *dongle)
{
    t_dongle   *smallest;

    if (coder->left_dongle->dongle_id
        < coder->right_dongle->dongle_id)
        smallest = coder->left_dongle;
    else
        smallest = coder->right_dongle;
    if (dongle != smallest)
    {
        pthread_mutex_lock(&smallest->lock);
        while (
            !is_stopped(coder->inputs)
            && smallest->request->size > 0
            && smallest->request->arr[0].id != coder->id
        )
            pthread_cond_wait(&smallest->cond, &smallest->lock);
        pthread_mutex_unlock(&smallest->lock);
    }
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
        request_dongle(coder, coder->right_dongle);
        request_dongle(coder, coder->left_dongle);
    }
    else
    {
        request_dongle(coder, coder->left_dongle);
        request_dongle(coder, coder->right_dongle);
    }
}

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

void take_both_dongles(t_coder *coder)
{
    t_dongle   *first;
    t_dongle   *second;
    long long  first_ready;
    long long  second_ready;

    if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
    while (!is_stopped(coder->inputs))
    {
        pthread_mutex_lock(&first->lock);
        pthread_mutex_lock(&second->lock);
        first_ready = first->last_compile_time
            + coder->inputs->dongle_cooldown;
        second_ready = second->last_compile_time
            + coder->inputs->dongle_cooldown;
        if (
            first->status == 0 && second->status == 0
            && first_ready <= time_in_ms() && second_ready <= time_in_ms()
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
            pthread_mutex_unlock(&second->lock);
            pthread_mutex_unlock(&first->lock);
            return ;
        }
        pthread_mutex_unlock(&second->lock);
        pthread_mutex_unlock(&first->lock);
    }
}
void *routine(void *arg)
{
    t_coder *coder;
    int		i;
    
    coder = arg;
    coder->last_compile_time = time_in_ms();
    i = 0;
    // printf("coder->id = %ld || right_dongle->dongle_id = %ld || left_donle->dongle_id = %ld\n", coder->id, coder->right_dongle->dongle_id, coder->left_dongle->dongle_id);

    while (
		i < coder->inputs->number_of_compiles_required
        && !is_stopped(coder->inputs)
	)
	{
        check_dongles(coder);
        take_both_dongles(coder);
        compiling(coder);
        release_dongle(coder);
        debugging(coder);
        refactoring(coder);
    	i++;
	}
    return NULL;
}
