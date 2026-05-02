#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// 1. Declare the mutex

void protected_function(int thread_id) {
    // 2. Lock the mutex before entering the critical section
    pthread_mutex_t lock;
    pthread_mutex_lock(&lock);

    printf("Thread %d: Entered protected function.\n", thread_id);
    sleep(1); // Simulate work
    printf("Thread %d: Leaving protected function.\n", thread_id);
    pthread_mutex_unlock(&lock);

    // 3. Unlock the mutex so others can enter
}

void* thread_runner(void* arg) {
    int id = *(int*)arg;
    protected_function(id);
    return NULL;
}

int main() {
    pthread_t threads[11];
    int ids[11] = {0,1,2,3,4,5,6,7,8,9,10};
    pthread_mutex_t lock;

    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    // Create two threads that call the same function
    for (int i = 0; i < 11; i++)
        pthread_create(&threads[i], NULL, thread_runner, &ids[i]);

    // Wait for both threads to finish
    for (int i = 0; i < 11; i++)
        pthread_join(threads[i], NULL);


    // 4. Destroy the mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
