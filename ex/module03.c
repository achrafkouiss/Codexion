#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct s_thread
{
    pthread_t id;
    int index;
} t_thread;

pthread_mutex_t lock;
pthread_cond_t cond;
int ready = 0;

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

// void *waiting(void *arg)
// {
//     t_thread *thread;

//     pthread_mutex_lock(&lock);
//     thread = (t_thread *) arg;
//     printf("Thread %d waiting\n", thread->index);
//     sleep(1);
//     pthread_cond_signal(&cond);
//     ready = 1;
//     pthread_mutex_unlock(&lock);
    
// }

void *process(void *arg)
{
    t_thread *thread;

    pthread_mutex_lock(&lock);
    thread = (t_thread *) arg;
    printf("Thread %d waiting\n", thread->index);
    while (ready == 0)
    pthread_cond_wait(&cond, &lock);
    pthread_mutex_unlock(&lock);
    printf("Thread %d process\n", thread->index);
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
    for (int i = 0; i < n; i++)
    {
        threads[i].index= i;
        pthread_create(&threads[i].id, NULL, process, &threads[i]);
    }
    sleep(1);
    pthread_mutex_lock(&lock);
    ready = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
    for (int i = 0; i < n; i++)
    pthread_join(threads[i].id, NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    free_threads(threads, n);
    return 0;
}



// #include <pthread.h>
// #include <stdio.h>
// #include <unistd.h>

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// int count = 0;
// const int COUNT_LIMIT = 5;

// void* watcher(void* arg) {
//     pthread_mutex_lock(&mutex);
//     // Use a while loop to handle "spurious wakeups"
//     while (count < COUNT_LIMIT) {
//         printf("Watcher: count is %d. Waiting...\n", count);
//         pthread_cond_wait(&cond, &mutex);
//     }
//     printf("Watcher: Threshold reached! Final count: %d\n", count);
//     pthread_mutex_unlock(&mutex);
//     return NULL;
// }

// void* worker(void* arg) {
//     for (int i = 0; i < 7; i++) {
//         pthread_mutex_lock(&mutex);
//         count++;
//         printf("Worker: Incremented count to %d\n", count);
//         if (count == COUNT_LIMIT) {
//             pthread_cond_signal(&cond); // Wake up the watcher
//         }
//         pthread_mutex_unlock(&mutex);
//         sleep(1);
//     }
//     return NULL;
// }

// int main() {
//     pthread_t t1, t2;
//     pthread_create(&t1, NULL, watcher, NULL);
//     pthread_create(&t2, NULL, worker, NULL);
//     pthread_join(t1, NULL);
//     pthread_join(t2, NULL);
//     return 0;
// }

