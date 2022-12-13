#include <stdio.h>
#include <sys/time.h>
#include "time.h"
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[])
{
    int iterations = 5000;
    int hist[25] = {0};
    int counter = 0;
    int random;
    int sum = 0;
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    if (argc >= 2)
        iterations = atoi(argv[1]);

    srand(time(NULL));

    for (int i = 0; i < iterations; i++)
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
    gettimeofday(&stop, NULL);
    printf("The runtime is %lu us\n\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

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
    printf("The number of iterations is %d.\n", sum);

    return 0;
}
