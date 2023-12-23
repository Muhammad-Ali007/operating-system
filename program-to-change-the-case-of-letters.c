#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

#define BUFFER_SIZE 256

int main() {
    int pipe1[2]; // pipe for sending original message
    int pipe2[2]; // pipe for sending modified message

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // child process (process 2)

        // close unused ends of the pipes
        close(pipe1[1]);
        close(pipe2[0]);

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;

        // reading the original message from the first process
        bytesRead = read(pipe1[0], buffer, sizeof(buffer));
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // reverse the case of each character in the message
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (islower(buffer[i])) {
                buffer[i] = toupper(buffer[i]);
            } else if (isupper(buffer[i])) {
                buffer[i] = tolower(buffer[i]);
            }
        }

        // send the modified message back to the first process
        if (write(pipe2[1], buffer, bytesRead) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        // close the remaining ends of the pipes
        close(pipe1[0]);
        close(pipe2[1]);

        exit(EXIT_SUCCESS);
    } else {
        // parent process (process 1)

        // close unused ends of the pipes
        close(pipe1[0]);
        close(pipe2[1]);

	// defining the message buffer for user input
	char message[100];
	fgets(message, 100, stdin);
        ssize_t messageSize = sizeof(message);

        // send the original message to the second process
        if (write(pipe1[1], message, messageSize) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        char modifiedMessage[BUFFER_SIZE];
        ssize_t bytesRead;

        // reading the modified message from the second process
        bytesRead = read(pipe2[0], modifiedMessage, sizeof(modifiedMessage));
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // print the modified message
        printf("Modified Message: %.*s\n", (int)bytesRead, modifiedMessage);

        // close the remaining ends of the pipes
        close(pipe1[1]);
        close(pipe2[0]);

        //wait(NULL); // wait for the child process to finish

        exit(EXIT_SUCCESS);
    }
}

