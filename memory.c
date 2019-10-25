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
 * Allocates memory for a given pointer
 * using malloc. Handles error appropriately.
 * 
 * @param size The size of the memory to allocate
 * @return void* The pointer to the newly allocated object
 */
void *allocate(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        perror(__func__);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
    return ptr;
}