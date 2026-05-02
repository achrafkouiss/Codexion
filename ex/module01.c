#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

//ex01
// int counter = 0;

// void *free_threads(pthread_t **threads, int size)
// {
//     if (!threads)
//         return NULL;
//     for (int i = 0; i < size; i++)
//         free(threads[i]);
//     free(threads);
//     return (NULL);
// }

// pthread_t **allo_threads(int n)
// {
//     pthread_t **threads;

//     threads = malloc(sizeof(pthread_t *) * n);
//     if (!threads)
//         return NULL;
//     for (int i = 0; i < n; i++)
//     {
//         threads[i] = malloc(sizeof(pthread_t));
//         if (!threads[i])
//             return (free_threads(threads, i));
//     }
//     return threads;
// }

// void *increment_counter(void *arg)
// {
//     for (int i = 0; i < 100000; i++)
//         counter++;
//     return NULL;
// }

// int main(int ac, char *av[])
// {
//     pthread_t **threads;
//     int n;

//     if (ac < 2)
//         return 1;
//     n = atoi(av[1]);
//     if (n <= 0)
//         return 1;
//     threads = allo_threads(n);
//     for (int i = 0; i < n; i++)
//         pthread_create(threads[i], NULL, increment_counter, NULL);
//     for (int i = 0; i < n; i++)
//         pthread_join(*threads[i], NULL);
//     printf("Expected: %d * 100000\n", n);
//     printf("Got: %d \n", counter);
//     free_threads(threads, n);
//     return 0;
// }


//ex02 ex03 04 05 mixed
// int counter = 0;

// void *free_threads(pthread_t **threads, int size)
// {
//     if (!threads)
//         return NULL;
//     for (int i = 0; i < size; i++)
//         free(threads[i]);
//     free(threads);
//     return (NULL);
// }

// pthread_t **allo_threads(int n)
// {
//     pthread_t **threads;

//     threads = malloc(sizeof(pthread_t *) * n);
//     if (!threads)
//         return NULL;
//     for (int i = 0; i < n; i++)
//     {
//         threads[i] = malloc(sizeof(pthread_t));
//         if (!threads[i])
//             return (free_threads(threads, i));
//     }
//     return threads;
// }

// void *increment_counter(void *arg)
// {
//     int *local = malloc(sizeof(int));
//     *local = 0;
//     for (int i = 0; i < 100000; i++)
//     {
//         *local += 1;
//         // counter++;
//         // int tmp = counter;
//         // tmp = tmp + 1;
//         // counter = tmp;
//         // if (i % 100 == 0)
//         //     usleep(1);
//     }
//     // printf("local %d\n", *local);
//     return (void *)local;
// }

// int main(int ac, char *av[])
// {
//     pthread_t **threads;
//     int n;
//     int sum = 0;
//     void *local;

//     if (ac < 2)
//         return 1;
//     n = atoi(av[1]);
//     if (n <= 0)
//         return 1;
//     threads = allo_threads(n);
//     for (int i = 0; i < n; i++)
//         pthread_create(threads[i], NULL, increment_counter, NULL);
//     for (int i = 0; i < n; i++)
//     {
//         pthread_join(*threads[i], &local);
//         sum += *((int*)local);
//         free(local);
//     }
//     printf("Expected: %d * 100000\n", n);
//     printf("Got: %d \n", sum);
//     free_threads(threads, n);
//     return 0;
// }


//ex06
typedef struct s_thread {
    pthread_t thread;
    int index;
    int *local;
} t_thread;

void *free_threads(t_thread **threads, int size)
{
    if (!threads)
        return NULL;
    for (int i = 0; i < size; i++)
    {
        free(threads[i]->local);
        free(threads[i]);
    }
    free(threads);
    return (NULL);
}

t_thread **allo_threads(int n)
{
    t_thread **threads;

    threads = malloc(sizeof(t_thread *) * n);
    if (!threads)
    return NULL;
    for (int i = 0; i < n; i++)
    {
        threads[i] = malloc(sizeof(t_thread));
        if (!threads[i])
        return (free_threads(threads, i));
        threads[i]->local = malloc(sizeof(int));
        if (!threads[i]->local)
        return (free_threads(threads, i));
    }
    return threads;
}

void *increment_counter(void *arg)
{
    t_thread *threads;

    threads = (t_thread *) arg;
    for (int i = 0; i < 100000; i++)
    {
        *(threads->local) += 1;
    }
    printf("Thread %d incremented counter to %d\n", threads->index, *(threads->local));
    return NULL;
}

int main(int ac, char *av[])
{
    t_thread **threads;
    int n;
    int sum = 0;
    void *local;
    int counter = 0;

    if (ac < 2)
        return 1;
    n = atoi(av[1]);
    if (n <= 0)
        return 1;
    threads = allo_threads(n);
    for (int i = 0; i < n; i++)
    {
        threads[i]->index = counter++;
        *(threads[i]->local) = 0;
        pthread_create(&(threads[i]->thread), NULL, increment_counter, threads[i]);
    }
    for (int i = 0; i < n; i++)
    {
        pthread_join(threads[i]->thread, NULL);
        sum = sum + *(threads[i]->local);
    }
    printf("Expected: %d * 100000\n", n);
    printf("Got: %d \n", sum);
    free_threads(threads, n);
    return 0;
}