#include "spath.h"

char *get_filename(char *filename) {
    // Strip directory slashes if any
    int index = 0;
    int len = strlen(filename);
    for (int i = len; i >= 0; i--) {
        if (filename[i] == '/') {
            index = i + 1;
        }
    }
    // Perform .in to .out conversion
    char *result = allocate((len - index + 1) * sizeof(char));
    int i = 0;
    while (filename[index] != '.') {
        result[i++] = filename[index++];
    }
    strncpy(&result[i], ".out", 4 * sizeof(char));
    result[i+4] = '\0';

    return result;
}

void output(FILE *fp) {
    fprintf(fp, "%d", numV);
    for (int i = 0; i < numV * numV; i++) {
        fprintf(fp, " %d", spaths[i]);
    }
    fprintf(fp, "\n");
}