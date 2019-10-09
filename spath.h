/**
 * CITS3402: High Performance Computing Project 2
 * Parallel algorithms to solve all pair shortest path
 *
 * @author Bruce How (22242664)
 * @date 03/10/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <omp.h>
#include <mpi.h>

#define p() printf("here\n"); fflush(stdout);
#define pint(x) printf("%s=%d\n", #x, x); fflush(stdout);
#define pstr(x) printf("%s=%s\n", #x, x); fflush(stdout);

#define MEMSIZ 8
#define MASTER 0
#define FROM_MASTER 0
#define FROM_WORKER 1

extern void process_graph(FILE *fp, int **weights, int *numV);

extern void *allocate(size_t size);
extern void *reallocate(void* ptr, size_t size);

extern void dijkstra(int node, int *weights, int numV, int offset, int **spaths);

extern void output(FILE *fp, int *spaths, int numV);
extern char *get_filename(char *filename);