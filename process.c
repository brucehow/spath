#include "spath.h"

/**
 * Processes a given graph by reading the binary input
 * accordingly. The first int represents numV while the
 * reamining ints are the weights.
 * 
 * @param fp The file pointer for the graph input
 * @param weights The pointer to a 1D int array to store the weights
 * @param numV The pointer to the int to store the number of vertices
 */
void process_graph(FILE *fp, int **weights, int *numV) {
    fread(numV, sizeof(int), 1, fp);
    *weights = allocate(*numV * *numV * sizeof(int));
    fread(*weights, sizeof(int), *numV * *numV, fp);
}