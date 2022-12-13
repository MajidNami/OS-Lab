#include <stdio.h>

typedef struct {
    int pid;
    int bt;
    int wt;
    int tt;
    int pr;
} process;

#define sort(array, n, member) (                        \
{                                                       \
    int c, d;                                           \
    process swap;                                       \
    for (c = 0 ; c < n - 1; c++)                        \
    {                                                   \
        for (d = 0 ; d < n - c - 1; d++)                \
        {                                               \
            if (array[d].member > array[d+1].member)    \
            {                                           \
                swap       = array[d];                  \
                array[d]   = array[d+1];                \
                array[d+1] = swap;                      \
            }                                           \
        }                                               \
    }                                                   \
} )

#define calc_average(processes, n, member)( \
{                                           \
    int sum = 0,i;                            \
    for ( i = 0; i < n; ++i)             \
        sum += processes[i].member;         \
    (((double) sum) / n);                   \
    }                                       \
)


void print_process(process p, int alg_num)
{
    printf("PID: %3d\t|\t", p.pid);
    printf("Burst Time: %3d\t|\t", p.bt);
    printf("Waiting Time: %3d\t|\t", p.wt);
    printf("Turnaround Time: %3d\t|\t", p.tt);
    printf("Priority: %3d\n", p.pr);
}

// Shortest Job First
int main()
{
    int n, alg_num;
    int j,i;
    process processes[100];
    printf("Priority\n\n");
    alg_num=1;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("\n");
    for ( j = 0; j < n; ++j)
    {
        printf("Enter burst time for process #%d: ", j);
        int burst_time;
        scanf("%d", &burst_time);


        printf("Enter priority number for process #%d: ", j);
        int prior_num;
        scanf("%d", &prior_num);
        processes[j].pr = prior_num;

        processes[j].pid = j;
        processes[j].bt = burst_time;
    }


    if (alg_num == 1)
        sort(processes, n, pr);
    else
    {
        printf("Error!\n");
        return -1;
    }

    processes[0].wt = 0;
    processes[0].tt = processes[0].bt;

    for ( i = 1; i < n; ++i)
    {
        processes[i].wt = processes[i-1].tt;
        processes[i].tt = processes[i].wt + processes[i].bt;
    }

    printf("\n");
    for ( i = 0; i < n; ++i)
        print_process(processes[i], alg_num);

    printf("\nAverage waiting time is: %g\n", calc_average(processes, n, wt));
    printf("Average turnaround time is: %g\n", calc_average(processes, n, tt));
    return 0;
}
