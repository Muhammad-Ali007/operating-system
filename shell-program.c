#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define MAX_ARGS 64
#define MAX_COMMANDS 16

void execute_command(char **args, int input_fd, int output_fd) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        if (output_fd != STDOUT_FILENO) {
            close(output_fd);
        }

        if (input_fd != STDIN_FILENO) {
            close(input_fd);
        }

        // If the command is not running in the background, wait for it to complete
        if (args[0][0] != '&') {
            waitpid(pid, NULL, 0);
        }
    }
}

int main() {
    char input[1024];
    char *args[MAX_ARGS];
    char *commands[MAX_COMMANDS];

    while (1) {
        // Print shell prompt
        printf("myshell> ");

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit if Ctrl+D (EOF) is pressed
        }

        // Remove newline character
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // Tokenize input into commands
        char *token;
        int num_commands = 0;
        token = strtok(input, "|");

        while (token != NULL && num_commands < MAX_COMMANDS) {
            commands[num_commands++] = token;
            token = strtok(NULL, "|");
        }

        // Execute each command
        int input_fd = STDIN_FILENO;
        for (int i = 0; i < num_commands; i++) {
            // Check for input/output redirection
            char *args[MAX_ARGS];
            int output_fd = STDOUT_FILENO;

            // Tokenize command into arguments
            token = strtok(commands[i], " ");
            int num_args = 0;
            while (token != NULL && num_args < MAX_ARGS) {
                args[num_args++] = token;
                token = strtok(NULL, " ");
            }
            args[num_args] = NULL;

            // Check for output redirection
            for (int j = 0; j < num_args; j++) {
                if (strcmp(args[j], ">") == 0) {
                    if (j + 1 < num_args) {
                        output_fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (output_fd == -1) {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }
                        args[j] = NULL; // Remove ">" from args
                        break;
                    } else {
                        fprintf(stderr, "Error: Missing filename after \">\"\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }

            execute_command(args, input_fd, output_fd);
            input_fd = output_fd; // Set the output of the previous command as the input for the next
        }
    }

    return 0;
}

