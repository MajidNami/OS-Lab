#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROCESSES 5


int main(int argc, char const *argv[])
{
    int id;
    int base_pid;
    int *hist;
    int sum = 0;
    int iterations = 5000;
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    if (argc >= 2)
        iterations = atoi(argv[1]);

    srand(time(NULL));

    id = shmget(IPC_PRIVATE, sizeof(int) * 25, IPC_CREAT | 0666);
    hist = (int *) shmat(id, NULL, 0);

    base_pid = getpid();
    for (int i = 0; i < PROCESSES; i++)
        if (getpid() == base_pid)
            fork();
        else
            break;

    if (getpid() == base_pid)
        for (int i = 0; i < PROCESSES; i++)
            wait(NULL);
    else
    {
        int counter, random;
        for (int i = 0; i < iterations / PROCESSES; i++)
        {
            counter = 0;
            for (int j = 0; j < 12; j++)
            {
                random = rand() % 99 + 1;
                if (random >= 49)
                    counter++;
                else
                    counter--;
            }
            hist[counter + 12]++;
        }
        exit(0);
    }
    // Printing result
    gettimeofday(&stop, NULL);

    int scale = iterations / 100;
    for (int i = 0; i < 25; i++)
    {
        printf("%3d: %7d - ", i - 12, hist[i]);
        sum += hist[i];
        int max = hist[i] / scale;
        for(int j = 0; j < max; j++)
            printf("*");
        printf("\n");
    }
    printf("\nThe runtime is %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    printf("The number of iterations is %d.\n", sum);
    return 0;
}



