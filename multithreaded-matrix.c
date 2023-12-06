#include <stdio.h>
#include <pthread.h>

#define SIZE 3

int matrixA[SIZE][SIZE] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
int resultMatrix[SIZE][SIZE];

struct ThreadData {
    int row;
};

// Function to perform matrix multiplication for one row
void *multiplyRow(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    int row = data->row;

    for (int col = 0; col < SIZE; col++) {
        resultMatrix[row][col] = 0;
        for (int k = 0; k < SIZE; k++) {
            resultMatrix[row][col] += matrixA[row][k] * matrixA[k][col];
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[SIZE];
    struct ThreadData threadData[SIZE];

    // Create threads
    for (int i = 0; i < SIZE; i++) {
        threadData[i].row = i;
        pthread_create(&threads[i], NULL, multiplyRow, (void *)&threadData[i]);
    }

    // Join threads
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    printf("Result Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
