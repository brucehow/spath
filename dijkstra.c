#include "spath.h"

void dijkstra(int node, int *weights, int numV, int offset, int **spaths) {
    bool *spt =  allocate(numV * sizeof(bool)); // Keeps track if visitied

    for (int i = 0; i < numV; i++) {
        (*spaths)[i + offset] = -1;
        spt[i] = false;
    }
    (*spaths)[node + offset] = 0;
    
    int current = node;
    for (int i = 0; i < numV; i++) {
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
