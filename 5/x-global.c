#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int x = 1;
void forkexample()
{
    if (fork() == 0)
        printf("Child has x = %d\n", ++x);
    else
        printf("Parent has x = %d\n", --x);
}
int main()
{
    forkexample();
    return 0;
}
