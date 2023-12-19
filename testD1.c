#include <stdio.h>

int main() {
    FILE *file = fopen("testFile.txt", "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }
    while (1) {
    }
    fclose(file);
    return 0;
}