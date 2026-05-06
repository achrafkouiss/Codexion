#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// typedef struct s_thread
// {
//     pthread_t id;
//     int index;
// } t_thread;

// pthread_mutex_t lock;
// pthread_cond_t cond;
// int ready = 0;

// void *free_threads(t_thread *threads, int size)
// {
//     if (!threads)
//         return NULL;
//     free(threads);
//     return (NULL);
// }

// t_thread *allo_threads(int n)
// {
//     t_thread *threads;

//     threads = malloc(sizeof(t_thread) * n);
//     if (!threads)
//         return NULL;
//     return threads;
// }

// // void *waiting(void *arg)
// // {
// //     t_thread *thread;

// //     pthread_mutex_lock(&lock);
// //     thread = (t_thread *) arg;
// //     printf("Thread %d waiting\n", thread->index);
// //     sleep(1);
// //     pthread_cond_signal(&cond);
// //     ready = 1;
// //     pthread_mutex_unlock(&lock);
    
// // }

// void *process(void *arg)
// {
//     t_thread *thread;

//     thread = (t_thread *) arg;
//     printf("Thread %d waiting\n", thread->index);
//     pthread_mutex_lock(&lock);
//     while (ready == 0)
//     pthread_cond_wait(&cond, &lock);
//     pthread_mutex_unlock(&lock);
//     printf("Thread %d process\n", thread->index);
//     return NULL;
// }

// int main(int ac, char *av[])
// {
//     t_thread *threads;
//     int n;
    
//     if (ac < 2)
//     return 1;
//     n = atoi(av[1]);
//     if (n <= 0)
//     return 1;
//     threads = allo_threads(n);
//     pthread_mutex_init(&lock, NULL);
//     pthread_cond_init(&cond, NULL);
//     for (int i = 0; i < n; i++)
//     {
//         threads[i].index= i;
//         pthread_create(&threads[i].id, NULL, process, &threads[i]);
//     }
//     sleep(1);
//     pthread_mutex_lock(&lock);
//     ready = 1;
//     // pthread_cond_broadcast(&cond);
//     pthread_cond_signal(&cond);
//     pthread_mutex_unlock(&lock);
//     for (int i = 0; i < n; i++)
//     pthread_join(threads[i].id, NULL);
//     pthread_mutex_destroy(&lock);
//     pthread_cond_destroy(&cond);
//     free_threads(threads, n);
//     return 0;
// }





//ex02

typedef struct s_thread
{
    pthread_t id;
    int index;
} t_thread;

pthread_mutex_t lock;
pthread_cond_t cond;
int buffer;
int has_data = 0;

void *free_threads(t_thread *threads, int size)
{
    if (!threads)
        return NULL;
    free(threads);
    return (NULL);
}

t_thread *allo_threads(int n)
{
    t_thread *threads;

    threads = malloc(sizeof(t_thread) * n);
    if (!threads)
        return NULL;
    return threads;
}

void *producer(void *arg)
{
    t_thread *thread;

    pthread_mutex_lock(&lock);
    thread = (t_thread *) arg;
    pthread_mutex_lock(&lock);
    printf("xxxxxxxxxxxxxxxxxxxxxxx\n");
    while (buffer == 0 && has_data == 0)
        pthread_cond_wait(&cond, &lock);
    buffer == 1;
    has_data == 1;
    printf("Produced: %d\n", thread->index);
    return NULL;
}

void *consumer(void *arg)
{
    t_thread *thread;

    thread = (t_thread *) arg;
    // printf("Thread %d waiting\n", thread->index);
    pthread_mutex_lock(&lock);
    while (buffer == 1 && has_data == 1)
    pthread_cond_wait(&cond, &lock);
    printf("Consumed: %d\n", thread->index);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int ac, char *av[])
{
    t_thread *threads;
    int n;
    
    if (ac < 2)
    return 1;
    n = atoi(av[1]);
    if (n <= 0)
    return 1;
    threads = allo_threads(n);
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    buffer = 0;
    // for (int i = 0; i < n; i++)
    // {
        // threads[i].index= i;
    pthread_create(&threads[i].id, NULL, consumer, &threads[i]);
    pthread_create(&threads[i].id, NULL, producer, &threads[i]);
    // }
    sleep(1);
    pthread_mutex_lock(&lock);
    // buffer = 1;
    pthread_cond_broadcast(&cond);
    // pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    for (int i = 0; i < n; i++)
    pthread_join(threads[i].id, NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    free_threads(threads, n);
    return 0;
}


