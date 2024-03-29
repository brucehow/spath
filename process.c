/**
 * CITS3402: High Performance Computing Project 2
 * Parallel algorithms to solve all pair shortest path
 *
 * Clarifications
 * The program will automatically generate an ouput file.
 * The ouput file will be in binary.
 * The program requires the -f flag to specify the input file.
 *
 * @author Bruce How (22242664)
 * @date 25/10/2019
 */

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