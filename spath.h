/**
 * CITS3402: High Performance Computing Project 2
 * Parallel algorithms to solve all pair shortest path
 *
 * @author Bruce How (22242664)
 * @date 03/10/2019
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>

#define MEMSIZ 8

extern int *weights;
extern int *spaths;
extern int numV;

extern void process_graph(FILE *fp);

extern void *allocate(size_t size);
extern void *reallocate(void* ptr, size_t size);

extern void dijkstra(int node);

extern void output(FILE *fp);
extern char *get_filename(char *filename);