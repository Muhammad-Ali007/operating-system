
#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int completion_time;
};

void calculateWaitingTime(struct Process processes[], int n) {
    int i;
    processes[0].waiting_time = 0;

    for (i = 1; i < n; i++) {
        processes[i].waiting_time = processes[i - 1].completion_time - processes[i].arrival_time;
        if (processes[i].waiting_time < 0) {
            processes[i].waiting_time = 0;
        }
    }
}

float calculateAverageWaitingTime(struct Process processes[], int n) {
    int i;
    float total_waiting_time = 0;

    for (i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
    }

    return total_waiting_time / n;
}

int main() {
    int n;
    int i;
   
    printf("Enter the number of processes: ");
    scanf("%d", &n);
   
    struct Process processes[n];

    for (i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].arrival_time);
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burst_time);
    }

    // Sort processes by arrival time
    for (i = 0; i < n - 1; i++) {
        int j;
        for (j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    // Calculate completion time
    for (i = 0; i < n; i++) {
        processes[i].completion_time = processes[i].arrival_time;
        if (i > 0) {
            processes[i].completion_time += processes[i - 1].completion_time;
        }
        processes[i].completion_time += processes[i].burst_time;
    }

    // Calculate waiting times
    calculateWaitingTime(processes, n);

    // Calculate and print the average waiting time
    float average_waiting_time = calculateAverageWaitingTime(processes, n);
    printf("Execution order:\n");
    for (i = 0; i < n; i++) {
        printf("Process %d\n", processes[i].pid);
    }
    printf("Average Waiting Time: %.2f\n", average_waiting_time);

    return 0;
}
