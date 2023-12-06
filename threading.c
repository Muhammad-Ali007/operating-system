#include <stdio.h>
#include <pthread.h>

void *task1(void *arg) {
    // Task 1: Print a message
    printf("Thread 1: Hello from Task 1!\n");

    // Other operations for Task 1

    pthread_exit(NULL);
}

void *task2(void *arg) {
    // Task 2: Print a message
    printf("Thread 2: Hello from Task 2!\n");

    // Other operations for Task 2

    pthread_exit(NULL);
}


int main() {
    pthread_t thread1, thread2;

    // Create the first thread
    if (pthread_create(&thread1, NULL, task1, NULL) != 0) {
        fprintf(stderr, "Error creating Thread 1\n");
        return 1;
    }

    // Create the second thread
    if (pthread_create(&thread2, NULL, task2, NULL) != 0) {
        fprintf(stderr, "Error creating Thread 2\n");
        return 1;
    }

    // Wait for both threads to finish
    if (pthread_join(thread1, NULL) != 0) {
        fprintf(stderr, "Error joining Thread 1\n");
        return 1;
    }

    if (pthread_join(thread2, NULL) != 0) {
        fprintf(stderr, "Error joining Thread 2\n");
        return 1;
    }

    return 0;
}

