#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_NUMBERS 50

struct ThreadData {
    int* numbers;
    int size;
    double average;
    int maximum;
    int minimum;
};

// function to calculate average of numbers
void* calculateAverage(void* data) {
    struct ThreadData* threadData = (struct ThreadData*)data;

    for (int i = 0; i < threadData->size; ++i) {
        threadData->average += threadData->numbers[i];
    }

    threadData->average /= threadData->size;

    printf("Thread for average calculation : %lu \n", pthread_self());
    pthread_exit(NULL);
}

// function to find the maximum
void* calculateMaximum(void* data) {
    struct ThreadData* threadData = (struct ThreadData*)data;

    threadData->maximum = threadData->numbers[0];

    for (int i = 1; i < threadData->size; ++i) {
        if (threadData->numbers[i] > threadData->maximum) {
            threadData->maximum = threadData->numbers[i];
        }
    }

    printf("Thread for maximum calculation : %lu \n", pthread_self());
    pthread_exit(NULL);
}

// function to find the minimum
void* calculateMinimum(void* data) {
    struct ThreadData* threadData = (struct ThreadData*)data;

    threadData->minimum = threadData->numbers[0];

    for (int i = 1; i < threadData->size; ++i) {
        if (threadData->numbers[i] < threadData->minimum) {
            threadData->minimum = threadData->numbers[i];
        }
    }

    printf("Thread for minimum calculation : %lu \n", pthread_self());
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num1> <num2> ... <numN>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int size = argc - 1;
    if (size > MAX_NUMBERS) {
        fprintf(stderr, "Maximum numbers allowed are %d\n", MAX_NUMBERS);
        return EXIT_FAILURE;
    }

    // allocating memory for numbers array
    int* numbers = (int*)malloc(size * sizeof(int));
    if (numbers == NULL) {
        perror("Got memory allocation error");
        return EXIT_FAILURE;
    }

    // parse command line arguments
    for (int i = 0; i < size; ++i) {
        numbers[i] = atoi(argv[i + 1]);
    }

    struct ThreadData data = {numbers, size, 0.0, 0, 0};
    pthread_t threads[3];

    // create threads
    if (pthread_create(&threads[0], NULL, calculateAverage, (void*)&data) != 0 ||
        pthread_create(&threads[1], NULL, calculateMaximum, (void*)&data) != 0 ||
        pthread_create(&threads[2], NULL, calculateMinimum, (void*)&data) != 0) {
        perror("Error creating threads");
        free(numbers);
        return EXIT_FAILURE;
    }

    // wait for threads to finish
    for (int i = 0; i < 3; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error joining threads");
            free(numbers);
            return EXIT_FAILURE;
        }
    }

    // print results
    printf("Average of Numbers: %.2f\n", data.average);
    printf("Maximum of Numbers: %d\n", data.maximum);
    printf("Minimum of Numbers: %d\n", data.minimum);

    // free allocated memory
    free(numbers);

    return EXIT_SUCCESS;
}
