// The program will by default output results to an output file

#include "spath.h"

int *weights;
int *spaths;
int numV;

void usage(char *err) {
    char *usage = "\nusage: spath -f input.in\n";
    if (err != NULL) {
        fprintf(stderr, "%s", err);
    }
    printf("%s", usage);
}

int main(int argc, char *argv[]) {
    if (argc < 3) { // CLA processing
        usage("invalid number of arguments supplied\n");
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1], "-f") != 0) {
        usage("expected -f parameter as the second argument\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[2], "rb"); // File checking
    if (fp == NULL) {
        fprintf(stderr, "%s: no such file\n", argv[2]);
		exit(EXIT_FAILURE);
	}
    process_graph(fp);
    spaths = allocate(numV * numV * sizeof(int));

    for (int i = 0; i < numV; i++) {
        dijkstra(i);
    }
    
    // Output
    char *filename = get_filename(argv[2]);
    FILE *fout = fopen(filename, "w");
    if (fout == NULL) {
        fprintf(stderr, "spath: failed to write to the output file\n");
        exit(EXIT_FAILURE);
    }
    output(fout);

    // Memory clearing
    fclose(fp);
    fclose(fout);
    free(spaths);
    free(weights);
    free(filename);
}