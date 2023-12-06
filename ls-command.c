#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    DIR *directory;
    struct dirent *dp;

    // Open the current directory
    directory = opendir(".");

    if (directory == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    // Read and print directory entries
    while ((dp = readdir(directory)) != NULL) {
        printf("Name: %s\n", dp->d_name);
        printf("Inode: %ld\n", (long)dp->d_ino);
        printf("----------\n");
    }

    // Close the directory
    closedir(directory);

    return 0;
}
