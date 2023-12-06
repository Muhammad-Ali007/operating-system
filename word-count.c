#include <stdio.h>

int main() {
    // open the text file in reading mode
    FILE *file;
    file = fopen("sample.txt", "r");

    if (file == NULL) {
        perror("Failed to open the file");
        return 1;
    }

    int spaceCount = 0;
    int lineCount = 0;
    int charCount = 0;
    char c;

    while ((c = fgetc(file)) != EOF) {
        charCount++;

        if (c == ' ') {
            spaceCount++;
        }

        if (c == '\n') {
            lineCount++;
        }
    }

    fclose(file);

    printf("Spaces: %d\n", spaceCount);
    printf("Lines: %d\n", lineCount);
    printf("Characters: %d\n", charCount);

    return 0;
}
