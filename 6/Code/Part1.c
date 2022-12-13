#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>

typedef struct Shared Shared;


struct Shared
{
    int count, rc;
    pthread_mutex_t mutex;
    pthread_mutex_t rw_mutex;
};

int shmid;

void reader() 
{
    Shared* shared = (Shared *)shmat(shmid, NULL, 0);
    int pid = getpid();

    bool done = 0;
    while (!done) 
    {
        pthread_mutex_lock(&(shared->mutex));
        (shared->rc)++;
        if (shared->rc == 1) 
            pthread_mutex_lock(&(shared->rw_mutex));
        pthread_mutex_unlock(&(shared->mutex));

        printf("Reader:\tPID: %d\tcount: %d\n", pid, shared->count);
        if (shared->count >= 8) 
            done = true;

        pthread_mutex_lock(&(shared->mutex));
        (shared->rc)--;
        if (shared->rc == 0) 
            pthread_mutex_unlock(&(shared->rw_mutex));
        pthread_mutex_unlock(&(shared->mutex));
        sleep(0.1);
    }

    shmdt(shared);
}

void writer() 
{
    Shared* shared = (Shared *)shmat(shmid, NULL, 0);
    int pid = getpid();

    bool done = 0;
    while (!done) 
    {
        pthread_mutex_lock(&(shared->rw_mutex));

        shared->count++;
        printf("Writer:\tPID: %d\tcount: %d\n", pid, shared->count);
        if (shared->count >= 8)
            done = true;
        pthread_mutex_unlock(&(shared->rw_mutex));
        sleep(0.5);
    }

    shmdt(shared);
}

int main(int argc, char const *argv[])
{
    int pid;
    int base_pid = getpid();

    shmid = shmget(IPC_PRIVATE, sizeof(Shared), IPC_CREAT | 0666);

    Shared* sh_at;
    sh_at = (Shared *)shmat(shmid, NULL, 0);
   

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&(sh_at->mutex), &attr);
    pthread_mutex_init(&(sh_at->rw_mutex), &attr);

    shmdt(sh_at);


    pid = fork();
    if (pid == 0) 
    { 
        writer();
        return 0;
    }

    for (int i = 0; i < 2; i++)
        if (getpid() == base_pid)
            pid = fork();
        else
            break;

    if (pid == 0) 
    { 
        reader();
        return 0;
    }


    if (getpid() == base_pid) 
    {
        wait(NULL);
        for (int i = 0; i < 2; i++) 
            wait(NULL);
    }

    shmctl(shmid, IPC_RMID, NULL);
}