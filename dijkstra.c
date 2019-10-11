#include "spath.h"

/**
 * Runs the Dijkstra algorithm for a given node. The spaths resulting
 * array is modified directly hence the double pointer parameter.
 * To further speed up the process, OMP parralellism has been implemented.
 * 
 * @param node The node to calculate the shortest paths from
 * @param weights The list of weights for all nodes to another
 * @param numV The number of nodes
 * @param offset The offset to store the results to for spaths
 * @param spaths The shortest path variable to store the results to
 */
void dijkstra(int node, int *weights, int numV, int offset, int **spaths) {
    bool *spt =  allocate(numV * sizeof(bool)); // Keeps track if visitied

    for (int i = 0; i < numV; i++) {
        (*spaths)[i + offset] = -1;
        spt[i] = false;
    }
    (*spaths)[node + offset] = 0;
    
    int current = node;
    int i;

    #pragma omp parallel for shared(spaths)
    for (i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            // Exclude self distance and unconnected nodes
            int direct = weights[(numV * current) + j]; // Direct distance to node
            if (j == current || direct == 0) {
                continue;
            }
            int dist = (*spaths)[offset + current] + direct;
            if ((*spaths)[offset + j] == -1 || dist < (*spaths)[offset + j]) {
                (*spaths)[offset + j] = dist;
            }
        }
        // Identify next node
        spt[current] = true;
        int lowest = -1;
        for (int k = 0; k < numV; k++) {
            if (!spt[k] && (*spaths)[offset + k] != -1 && (lowest == -1 || (*spaths)[offset + k] < lowest)) {
                lowest = (*spaths)[offset + k];
                current = k;
            }
        }
    }
    free(spt);
}
