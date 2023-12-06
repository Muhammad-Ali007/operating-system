#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    //char *cmd[] = {"fold", "-w", "15", "MuhammadAliIrfan.txt", NULL};
    char *cmd[] = {"fold", "-s", "-w", "15", "MuhammadAliIrfan.txt", NULL};

    execvp(cmd[0], cmd);

    perror("execvp");
    return 1;
}
