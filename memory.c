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