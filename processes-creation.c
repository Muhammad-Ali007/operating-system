#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // Create task1
    pid_t task1 = getpid();
    
    printf("Task1 (PID %d) is the parent process.\n", task1);
    
    // Create task2
    pid_t task2 = fork();
    
    if (task2 == 0) {
        // This code executes in task2 (child of task1)
        printf("Task2 (PID %d) is the child of Task1 (PID %d).\n", getpid(), task1);
        
        // Create task3
        pid_t task3 = fork();
        
        if (task3 == 0) {
            // This code executes in task3 (child of task2)
            printf("Task3 (PID %d) is the child of Task2 (PID %d).\n", getpid(), task2);
        } else if (task3 > 0) {
            // This code executes in task2 (parent of task3)
            wait(NULL); // Wait for task3 to complete
        } else {
            perror("Failed to create Task3.");
            exit(EXIT_FAILURE);
        }
    } else if (task2 > 0) {
        // This code executes in task1 (parent of task2)
        wait(NULL); // Wait for task2 to complete
        
        // Create task4
        pid_t task4 = fork();
        
        if (task4 == 0) {
            // This code executes in task4 (child of task1)
            printf("Task4 (PID %d) is the child of Task1 (PID %d).\n", getpid(), task1);
        } else if (task4 > 0) {
            // This code executes in task1 (parent of task4)
            wait(NULL); // Wait for task4 to complete
        } else {
            perror("Failed to create Task4.");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("Failed to create Task2.");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
