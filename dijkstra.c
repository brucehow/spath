#include "spath.h"

void dijkstra(int node) {
    bool *spt =  allocate(numV * sizeof(bool)); // Keeps track if visitied

    for (int i = 0; i < numV; i++) {
        spaths[(node * numV) + i] = -1;
        spt[i] = false;
    }
    spaths[(node * numV) + node] = 0;
    
    int current = node;
    for (int i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            // Exclude self distance and unconnected nodes
            int direct = weights[(numV * current) + j]; // Direct distance to node
            if (j == current || direct == 0) {
                continue;
            }
            int dist = spaths[(node * numV) + current] + direct;
            if (spaths[(node * numV) + j] == -1 || dist < spaths[(node * numV) + j]) {
                spaths[(node * numV) + j] = dist;
            }
        }
        // Identify next node
        spt[current] = true;
        int lowest = -1;
        for (int k = 0; k < numV; k++) {
            if (!spt[k] && spaths[(node * numV) + k] != -1 && (lowest == -1 || spaths[(node * numV) + k] < lowest)) {
                lowest = spaths[(node * numV) + k];
                current = k;
            }
        }
    }
    free(spt);
}
