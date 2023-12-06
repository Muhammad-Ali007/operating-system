#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];  // File descriptors for the pipe

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Fork the first child process for "ls -l"
    pid_t ls_pid = fork();

    if (ls_pid < 0) {
        perror("Fork for 'ls -l' failed");
        exit(EXIT_FAILURE);
    } else if (ls_pid == 0) {

        close(pipefd[0]);

        // Redirect stdout to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[1]);
        
        execlp("ls", "ls", "-l", (char *)NULL);

        perror("ls -l execution failed");
        exit(EXIT_FAILURE);
    }

    // Fork the second child process for "sort -n 4"
    pid_t sort_pid = fork();

    if (sort_pid < 0) {
        perror("Fork for 'sort -n +4' failed");
        exit(EXIT_FAILURE);
    } else if (sort_pid == 0) {

        // Close the write end of the pipe
        close(pipefd[1]);

        // Redirect stdin to the read end of the pipe
        dup2(pipefd[0], STDIN_FILENO);

        close(pipefd[0]);

        // Execute 'sort -n 4'
        //execlp("sort", "sort", "-n", "+4", (char *)NULL);
	execlp("grep", "grep", "round", (char *)NULL);

        perror("sort -n 4 execution failed");
        exit(EXIT_FAILURE);
    }

    // Close both ends of the pipe in the parent process
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for the child processes to finish
    wait(NULL);
    wait(NULL);

    return 0;
}
