#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//ex00
// int counter = 0;
int counter1 = 0;
int counter2 = 0;
pthread_mutex_t lock1;
pthread_mutex_t lock2;

void *free_threads(pthread_t **threads, int size)
{
    if (!threads)
        return NULL;
    for (int i = 0; i < size; i++)
        free(threads[i]);
    free(threads);
    return (NULL);
}

pthread_t **allo_threads(int n)
{
    pthread_t **threads;

    threads = malloc(sizeof(pthread_t *) * n);
    if (!threads)
        return NULL;
    for (int i = 0; i < n; i++)
    {
        threads[i] = malloc(sizeof(pthread_t));
        if (!threads[i])
            return (free_threads(threads, i));
    }
    return threads;
}

// void *increment_counter(void *arg)
// {
//     for (int i = 0; i < 100000; i++)
//     {
//         pthread_mutex_lock(&lock);
//         counter++;
//         pthread_mutex_unlock(&lock);
//     }
//     return NULL;
// }

void *increment_counter(void *arg)
{
    srand(time(NULL));
    int randomNum = rand();

    if (randomNum % 2 == 0)
    {
        pthread_mutex_lock(&lock1);
        for (int i = 0; i < 100000; i++)
        counter1++;
        pthread_mutex_unlock(&lock2);
    }
    else
    {
        pthread_mutex_lock(&lock2);
        for (int i = 0; i < 100000; i++)
        counter2++;
        pthread_mutex_unlock(&lock1);
    }
    return NULL;
}

// void *increment_counter(void *arg)
// {
//     pthread_mutex_lock(&lock1);
//     usleep(100); // important to force timing
//     pthread_mutex_lock(&lock2);
//     for (int i = 0; i < 100000; i++)
//     {
//         counter1++;
//         counter2++;
//     }
//     pthread_mutex_unlock(&lock1);
//     pthread_mutex_unlock(&lock2);
//     return NULL;
// }



int main(int ac, char *av[])
{
    pthread_t **threads;
    int n;
    
    if (ac < 2)
    return 1;
    n = atoi(av[1]);
    if (n <= 0)
    return 1;
    threads = allo_threads(n);
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock1, NULL);
    for (int i = 0; i < n; i++)
    pthread_create(threads[i], NULL, increment_counter, NULL);
    for (int i = 0; i < n; i++)
    pthread_join(*threads[i], NULL);
    printf("Expected: %d * 100000\n", n);
    printf("counter1: %d \n", counter1);
    printf("counter1: %d \n", counter2);
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    free_threads(threads, n);
    return 0;
}
