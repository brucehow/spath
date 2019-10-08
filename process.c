#include "spath.h"

void process_graph(FILE *fp) {
    fread(&numV, sizeof(int), 1, fp);
    weights = allocate(numV * numV * sizeof(int));
    
    for (int i = 0; i < numV * numV; i++) {
        fread(&weights[i], sizeof(int), 1, fp);
    }
}