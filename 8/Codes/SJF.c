#include <stdio.h>

typedef struct Process Process;

struct Process {
    int pid;
    int bt;
    int wt;
    int tt;
};

void sortByBurstTime(int taskNum, Process *processes) {
    for (int i = 0; i < taskNum; i++) {
        for (int j = i + 1; j < taskNum; j++) {
            if (processes[i].bt > processes[j].bt) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

int main() {
    int taskNum = 0;
    printf("Enter the number of tasks:\n");
    scanf("%d", &taskNum);
    Process processes[taskNum];
    for (int i = 0; i < taskNum; i++) {
        printf("Enter the burst time of P%d: ", i);
        processes[i].pid = i;
        scanf("%d", &processes[i].bt);
        processes[i].wt = i;
        processes[i].tt = i;
    }

    sortByBurstTime(taskNum, processes);


    processes[0].wt = 0;
    processes[0].tt = processes[0].bt;
    for (int i = 1; i < taskNum; i++) {
        processes[i].wt = processes[i - 1].wt + processes[i - 1].bt;
        processes[i].tt = processes[i].wt + processes[i].bt;
    }

    float sumTT = 0, sumWT = 0;
    printf("Process     BT          WT          TT\n");
    for (int i = 0; i < taskNum; i++) {
        printf("P%-11d%-12d%-12d%-12d\n", processes[i].pid, processes[i].bt, processes[i].wt, processes[i].tt);
        sumTT += (float)processes[i].tt;
        sumWT += (float)processes[i].wt;
    }

    printf("Average waiting time: %.2f\n Average Turn around time: %.2f\n", sumWT / taskNum, sumTT / taskNum);
    return 0;
}
