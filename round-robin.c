#include <stdio.h>

struct Process {
    int pid;       // Process ID
    int arrival;   // Arrival time
    int burst;     // Burst time
    int remaining; // Remaining burst time
    int waiting;   // Waiting time
};

int main() {
    int n, quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];

    // Input process details
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter Arrival Time for Process %d: ", i + 1);
        scanf("%d", &processes[i].arrival);
        printf("Enter Burst Time for Process %d: ", i + 1);
        scanf("%d", &processes[i].burst);
        processes[i].remaining = processes[i].burst;
        processes[i].waiting = 0;
    }

    printf("Enter the Time Quantum: ");
    scanf("%d", &quantum);

    int time = 0;
    int completed = 0;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].remaining > 0) {
                if (processes[i].remaining <= quantum) {
                    time += processes[i].remaining;
                    processes[i].waiting += time - processes[i].arrival - processes[i].burst;
                    processes[i].remaining = 0;
                    completed++;
                } else {
                    time += quantum;
                    processes[i].remaining -= quantum;
                }
            }
        }
    }

    // Calculate and display waiting times
    float totalWaitingTime = 0;
    printf("\nProcess Execution Order:\n");
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waiting;
        printf("P%d ", processes[i].pid);
    }

    printf("\n\nAverage Waiting Time: %.2f\n", totalWaitingTime / n);

    return 0;
}
