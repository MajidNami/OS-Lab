#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


pthread_mutex_t chopstick[6];

void *philosopher(int n)
{
    for (int i = 0; i < 2; i++)
    {
        printf("Philosopher %d is thinking!!!\n", n);

        pthread_mutex_lock(&chopstick[n - 1]);
        pthread_mutex_lock(&chopstick[n % 5]);

        printf("Philosopher %d is eating using chopstick(%d) and chopstick(%d)!!!\n", n, n - 1, n % 5);
        sleep(1);

        pthread_mutex_unlock(&chopstick[n - 1]);
        pthread_mutex_unlock(&chopstick[n % 5]);

        printf("Philosopher %d finished eating!!\n", n);
    }
}

int main()
{
    pthread_t threads[6];

    for (int i = 1; i <= 5; i++)
        pthread_mutex_init(&chopstick[i], NULL);

    for (int i = 1; i <= 5; i++)
        pthread_create(&threads[i], NULL, (void *) philosopher, (void *)(intptr_t) i);

    for (int i = 1; i <= 5; i++)
        pthread_join(threads[i], NULL);
}