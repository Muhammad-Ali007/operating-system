#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int sourceFile, destinationFile;
    char buffer[1024];

    // opening the source file in read-only mode
    sourceFile = open("source.txt", O_RDONLY);
    if (sourceFile == -1) {
        perror("Error opening source file");
        return 1;
    }

    // reading content from the source file
    ssize_t bytesRead = read(sourceFile, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("Error reading from source file");
        close(sourceFile);
        return 1;
    }
    // closing the source file
    close(sourceFile);

    // opening the destination file in write-only mode (create if it doesn't exist, truncate if it does)
    destinationFile = open("destination.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (destinationFile == -1) {
        perror("Error opening destination file");
        return 1;
    }

    // writing the content to the destination file
    ssize_t bytesWritten = write(destinationFile, buffer, bytesRead);
    if (bytesWritten == -1) {
        perror("Error writing to destination file");
        close(destinationFile);
        return 1;
    }
    // closing the destination file
    close(destinationFile);

    // deleting the previous file (source file)
    if (unlink("source.txt") != 0) {
        perror("Error deleting source file");
        return 1;
    }

    printf("Deleted source.txt file.\n");

    return 0;
}
