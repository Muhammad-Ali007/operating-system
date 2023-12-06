#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>  // Add this line for strlen function

#define BUFF 100     // Define the size of bufin

int main(void)
{
    int fd[2], bytesin;
    pid_t childpid;
    char bufin[BUFF] = "empty!\n";
    char bufout[] = " ";

    if (pipe(fd) == -1)
    {
        perror("pipe");  // Change "fork" to "pipe" in the error message
        return 1;
    }

    bytesin = strlen(bufin);
    childpid = fork();

    if (childpid == -1)
    {
        perror("fork");
        return 1;
    }

    if (childpid)
    {
        // Close the read end of the pipe in the parent
        close(fd[0]);

        // Write to the pipe
        write(fd[1], bufout, strlen(bufout) + 1);
    }
    else
    {
        // Close the write end of the pipe in the child
        close(fd[1]);

        // Read from the pipe
        bytesin = read(fd[0], bufout, sizeof(bufout));
    }

    printf("[%ld]: my bufin is {%.*s}, my bufout is {%s}\n", (long)getpid(), bytesin, bufin, bufout);
    return 0;
}
