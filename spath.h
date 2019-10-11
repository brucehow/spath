/**
 * CITS3402: High Performance Computing Project 2
 * Parallel algorithms to solve all pair shortest path
 *
 * Clarifications
 * The program will automatically generate an ouput file.
 * The ouput file will, like the input file, be in binary.
 *
 * @author Bruce How (22242664)
 * @date 11/10/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

// Constants for MPI
#define MASTER 0
#define FROM_MASTER 0 
#define FROM_WORKER 1

/**
 * Processes a given graph input and stores the list of weights
 * and number of vertices accordingly.
 * 
 * @param fp The file pointer for the graph input
 * @param weights The pointer to a 1D int array to store the weights
 * @param numV The pointer to the int to store the number of vertices
 */
extern void process_graph(FILE *fp, int **weights, int *numV);

/**
 * Allocate memory of a specified size for a given pointer.
 * 
 * @param size The size of the memory to allocate
 * @return void* The pointer to the newly allocated object
 */
extern void *allocate(size_t size);

/**
 * Perform Dijkstra's algorithm for calculating the shortest
 * paths from one node to the all other nodes.
 * 
 * @param node The node to calculate the shortest paths from
 * @param weights The list of weights for all nodes to another
 * @param numV The number of nodes
 * @param offset The offset to store the results to for spaths
 * @param spaths The shortest path variable to store the results to
 */
extern void dijkstra(int node, int *weights, int numV, int offset, int **spaths);

/**
 * Output the results of a list of shortest paths in binary to a 
 * specified file pointer.
 * 
 * @param fp The file pointer to output the results to
 * @param spaths The array of shortest paths
 * @param numV The number of nodes
 */
extern void output(FILE *fp, int *spaths, int numV);

/**
 * Get the output file name given an input filename.
 * 
 * @param filename The input filename
 * @return char* The new output filename
 */
extern char *get_filename(char *filename);

/**
 * Output the time taken to perform the the computation
 * 
 * @param start The starting timeval structure
 * @param end The ending timeval structure
 */
extern void output_time(struct timeval start, struct timeval end);
