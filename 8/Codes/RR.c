#include <stdio.h>

typedef struct Process Process;

struct Process {
    int pid;
    int bt;
    int wt;
    int tt;
    int startPoint;
    int endPoint;
    int remainTime;
};

int allDone(int taskNum, Process *processes) {
    for (int i = 0; i < taskNum; i++) {
        if (processes[i].remainTime != 0)
            return 0;
    }
    return 1;
}

void simulate(int taskNum, Process *processes, int timeQ) {
    int time = 0;
    for (int i = 0;; i++) {
        if (allDone(taskNum, processes))
            break;
        int turn = i % taskNum;
        if (processes[turn].remainTime == 0) {
            continue;
        }

        if (processes[turn].remainTime > timeQ) {
            if (processes[turn].startPoint == -1)
                processes[turn].startPoint = time;
            time += timeQ;
            processes[turn].remainTime -= timeQ;
        } else {
            time += processes[turn].remainTime;
            processes[turn].remainTime = 0;
            if (processes[turn].endPoint == -1)
                processes[turn].endPoint = time;
        }
    }

    for (int i = 0; i < taskNum; i++) {
        processes[i].wt = processes[i].endPoint - processes[i].bt;
        processes[i].tt = processes[i].endPoint;
    }
}

int main() {
    int taskNum = 0, timeQ = 0;
    printf("Enter the number of tasks:\n");
    scanf("%d", &taskNum);
    Process processes[taskNum];
    for (int i = 0; i < taskNum; i++) {
        printf("Enter the burst time of P%d: ", i);
        processes[i].pid = i;
        scanf("%d", &processes[i].bt);
        processes[i].wt = i;
        processes[i].tt = i;
        processes[i].remainTime = processes[i].bt;
        processes[i].startPoint = -1;
        processes[i].endPoint = -1;
    }

    printf("Enter Time quantum:\n");
    scanf("%d", &timeQ);


    simulate(taskNum, processes, timeQ);

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
