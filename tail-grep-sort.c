#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 100

int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() {
    int pipe_fd[2];
    pid_t child_pid;

    // Create pipe
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    if ((child_pid = fork()) == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process to read the last 5 lines
        close(pipe_fd[0]);  // Close unused read end

        // Redirect stdout to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);  // Close the original write end

        execlp("tail", "tail", "-n", "5", "alpha.txt", (char *)NULL);

        // Exec fails only if the command is not found
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process

        // Close the write end of the pipe
        close(pipe_fd[1]);

        // Fork another child process
        if ((child_pid = fork()) == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            // Child process to filter lines containing "ee"
            // Redirect stdin to the read end of the pipe
            dup2(pipe_fd[0], STDIN_FILENO);
            close(pipe_fd[0]);  // Close the original read end

            execlp("grep", "grep", "ee", (char *)NULL);

            // Exec fails only if the command is not found
            perror("Exec failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process

            // Close the read end of the pipe
            close(pipe_fd[0]);

            // Fork another child process
            if ((child_pid = fork()) == -1) {
                perror("Fork failed");
                exit(EXIT_FAILURE);
            }

            if (child_pid == 0) {
                // Child process to sort the lines
                // Redirect stdin to the read end of the pipe
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);  // Close the original write end

                execlp("sort", "sort", (char *)NULL);

                // Exec fails only if the command is not found
                perror("Exec failed");
                exit(EXIT_FAILURE);
            } else {
                // Parent process

                // Close the write end of the pipe
                close(pipe_fd[1]);

                // Wait for the child processes to finish
                wait(NULL);
                wait(NULL);
                wait(NULL);

                // End the main program
                exit(EXIT_SUCCESS);
            }
        }
    }

    return 0;
}

