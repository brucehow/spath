#include "spath.h"

/**
 * Produces the output filename by extracting the input 
 * filename (omitting any leading directories) and appending
 * the appropriate file extension. 
 *
 * @param filename The input filename 
 * @return char* The new output filename
 */
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

/**
 * Output the results of a list of shortest paths in binary to a 
 * specified file pointer. This is done using fwrite.
 * 
 * @param fp The file pointer to output the results to
 * @param spaths The array of shortest paths
 * @param numV The number of nodes
 */
void output(FILE *fp, int *spaths, int numV) {
    fwrite(&numV, sizeof(int), 1, fp);
    fwrite(spaths, sizeof(int), numV * numV, fp);
}

/**
 * Returns the difference between the start and end timevals
 * 
 * @param start The starting timeval structure
 * @param end The ending timeval structure
 */
void output_time(struct timeval start, struct timeval end) {
    float diff = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("Total computation time: %fs\n", diff);
}