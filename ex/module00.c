#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct s_thread {
    pthread_t thread;
    int index;
} t_thread;

void *index_sleep(void *arg)
{
    // printf("Thread %d started\n", ((t_thread *)arg)->index);
    printf("Thread %d started (tid=%lu)\n", ((t_thread *)arg)->index, pthread_self());
    usleep(((t_thread *)arg)->index * 100000);
    printf("Thread %d finished\n", ((t_thread *)arg)->index);
    return NULL;
}

int main(int ac, char *av[])
{
    t_thread *thread_s;
    int n;

    if (ac < 2)
        return 1;
    n = atoi(av[1]);
    if (n <= 0 || n >= 1000)
        return 1;
    thread_s = malloc(sizeof(t_thread) * n);
    if (!thread_s)
        return 1;
    for (int i = 0; i < n; i++)
    {
        thread_s[i].index = i;
        pthread_create(&thread_s[i].thread, NULL, index_sleep, &thread_s[i]);
    }
    for (int j = 0; j < n; j++)
        pthread_join(thread_s[j].thread, NULL);
    printf("All threads finished\n");
    free(thread_s);
}