#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 5
#define SLEEP_TIME 1

typedef enum
{
    SAFE,
    UNSAFE
} SystemState;

int request_resource(int customer_num, int *request);

int release_resource(int customer_num, int *request);

int *generate_random_request(int customer_num);

void initialize(int argc, char *argv[]);

SystemState safe();

int ls_eq(int *a, int *b, int size);

void *customer_handler(void *args);

int is_zero(int *a, int size);

int finish[NUMBER_OF_CUSTOMERS];

int work[NUMBER_OF_RESOURCES];

int available[NUMBER_OF_RESOURCES];

int maximum[NUMBER_OF_RESOURCES][NUMBER_OF_CUSTOMERS];

int allocation[NUMBER_OF_RESOURCES][NUMBER_OF_CUSTOMERS];

int need[NUMBER_OF_RESOURCES][NUMBER_OF_CUSTOMERS];

pthread_t threads[NUMBER_OF_CUSTOMERS];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[])
{
    srand(time(0));
    initialize(argc, argv);
    SystemState state = safe();
    if (state == SAFE)
    {
        printf("we are in the safe state !!!\n");
        printf("going to start working with clients\n");
        int indexes[NUMBER_OF_CUSTOMERS];
        for (size_t i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        {
            indexes[i] = i;
        }

        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        {
            pthread_create(&threads[i], NULL, customer_handler, (void *)(&indexes[i]));
        }
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
            pthread_join(threads[i], NULL);
    }
    else
    {
        printf("unsafe state \n");
        exit(EXIT_FAILURE);
    }
}

void initialize(int argc, char *argv[])
{
    if (argc < NUMBER_OF_RESOURCES + 1)
    {
        printf("not enough arguments\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        available[i] = strtol(argv[i + 1], NULL, 10);
        work[i] = available[i];
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        printf("av[%d]: %d\n", i, available[i]);

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        finish[i] = 0;

    printf("allocation is zero !!! \n");
    printf("maximum = need : \n");

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        for (int j = 0; j < NUMBER_OF_CUSTOMERS; j++)
        {
            allocation[i][j] = 0;
            maximum[i][j] = (rand() % available[i]) + 1;
            need[i][j] = maximum[i][j];
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}

SystemState safe()
{
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        if (!finish[i] && ls_eq(need[i], work, NUMBER_OF_RESOURCES))
        {
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                work[j] = work[j] + allocation[i][j];
            finish[i] = 1;
        }
    }
    SystemState is_safe = SAFE;
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        if (!finish[i])
            is_safe = UNSAFE;

    return is_safe;
}

int request_resource(int customer_num, int *request)
{

   

    if (!ls_eq(request, need[customer_num], NUMBER_OF_RESOURCES))
    {
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&lock);


    printf("request:{ ");
    for (size_t i = 0; i < NUMBER_OF_RESOURCES; i++)
        printf("%d ", request[i]);
    printf("} from customer: %d\n", customer_num);

    while (!ls_eq(request, available, NUMBER_OF_RESOURCES))
        pthread_cond_wait(&cond, &lock);

    printf("--------------------\n");
    printf("request from customer %d is ok \n", customer_num);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        available[i] = available[i] - request[i];

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        allocation[customer_num][i] = allocation[customer_num][i] + request[i];

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        need[customer_num][i] = need[customer_num][i] - request[i];

    printf("available: \n");
    for (size_t i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        printf("%d ", available[i]);
    }
    printf("\n");
    printf("--------------------\n");
    pthread_mutex_unlock(&lock);
    return 0;
}

int release_resource(int customer_num, int *request)
{
    pthread_mutex_lock(&lock);
    printf("request from customer %d released resources.\n", customer_num);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        available[i] = available[i] + request[i];

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        allocation[customer_num][i] = allocation[customer_num][i] - request[i];


    printf("need:{ ");
    for (size_t i = 0; i < NUMBER_OF_RESOURCES; i++)
        printf("%d ", need[customer_num][i]);
    printf("} from customer: %d\n", customer_num);

    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);

    return 1;
}

int ls_eq(int *a, int *b, int size)
{
    for (int i = 0; i < size; i++)
        if (a[i] > b[i])
            return 0;
    return 1;
}

int is_zero(int *a, int size)
{
    for (int i = 0; i < size; i++)
        if (a[i] != 0)
            return 0;
    return 1;
}

void *customer_handler(void *args)
{
    int customer_num = *((int *)args);
    while (!is_zero(need[customer_num], NUMBER_OF_RESOURCES))
    {
        pthread_mutex_lock(&lock);
        int *request = generate_random_request(customer_num);
        pthread_mutex_unlock(&lock);
        request_resource(customer_num, request);
        sleep(SLEEP_TIME);
        release_resource(customer_num, request);
        
    }
    return NULL;
}

int *generate_random_request(int customer_num)
{
    int *request = malloc(sizeof(int) * NUMBER_OF_RESOURCES);
    for (size_t i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        request[i] = rand() % (1 + need[customer_num][i]);
    }
    return request;
}