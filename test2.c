#include <stdio.h>
#include <pthread.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <stdlib.h>

void *t_index(void *arg)
{
    pthread_t *thread;


}

int main(int ac, char *av[])
{
    pthread_t *thread;
    int n;
    int i;
    int j;
    
    n = 5;
    i = 0;
    j = 0;
    thread = malloc(sizeof(pthread_t) * n);
    while (i < n)
    {
        pthread_create(&thread[i], NULL, t_index, thread[i]);
        i++;
    }
    while (j < n)
        pthread_join(&thread[j++], NULL);
}