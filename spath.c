#include "spath.h"

/**
 * Produces a standard usage error output to stderr.
 * There is an optional parameter to display an additional
 * error message.
 * 
 * @param err The optional error message
 */
void usage(char *err) {
    char *usage = "\nusage: spath -f input.in\n";
    if (err != NULL) {
        fprintf(stderr, "%s", err);
    }
    printf("%s", usage);
}

/**
 * Main function that checks that the user specified CLA are valid.
 * Processes the graph and appropriately run the Dijkstra's algorithm
 * using these variables with MPI. 
 *
 * @param argc The number of CLA
 * @param argv The list of arguments
 * @return int The exit type
 */
int main(int argc, char *argv[]) {
     // CLA processing
    if (argc < 3) {
        usage("invalid number of arguments supplied\n");
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1], "-f") != 0) {
        usage("expected -f parameter as the second argument\n");
        exit(EXIT_FAILURE);
    }

    // File validation
    FILE *fp = fopen(argv[2], "rb");
    if (fp == NULL) {
        fprintf(stderr, "%s: no such file\n", argv[2]);
		exit(EXIT_FAILURE);
	}

    int *weights; // 1D array containing the list of weights
    int numV; // Number of vertices/nodes
    int *spaths; // Result variable
    struct timeval start, end; // Timing variables

    // MPI initialisation
    MPI_Status status;
    int numtasks, numworkers, taskid, nodes, offset = 0;

    if (MPI_Init(&argc, &argv) != 0) {
        fprintf(stderr, "error initialising MPI");
        exit(EXIT_FAILURE);
    }
    
    // Gets the number of tasks and the current taskid to determine master
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    numworkers = numtasks - 1;

    if (taskid == MASTER) {
        process_graph(fp, &weights, &numV);
        spaths = allocate(numV * numV * sizeof(int));

        // Variables for split work
        int avgV = numV / numworkers;
        int extra = numV % numworkers;

        // Start the timer before Dijkstra computation
        gettimeofday(&start, NULL); 
        
        // Send tasks to workers
        for (int dest = 1; dest <= numworkers; dest++) {
            nodes = avgV + (dest <= extra); // Optimal way for workload split
            // Sends the appropriate variables to the workers
            MPI_Send(&nodes, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&numV, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(weights, numV * numV, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            offset += numV * nodes;
        }

        // Receive tasks from workers
        for (int source = 1; source <= numworkers; source++) {
            MPI_Recv(&nodes, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            MPI_Recv(&offset, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            MPI_Recv(&spaths[offset], numV * nodes, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
        }

        gettimeofday(&end, NULL);

        // Output
        char *filename = get_filename(argv[2]);
        FILE *fout = fopen(filename, "w");
        if (fout == NULL) {
            fprintf(stderr, "spath: failed to write to the output file\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            exit(EXIT_FAILURE);
        }
        output(fout, spaths, numV);
        output_time(start, end);

        fclose(fp);
        fclose(fout);
        free(spaths);
        free(weights);
        free(filename);
    } else {
        MPI_Recv(&nodes, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&numV, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        weights = allocate(numV * numV * sizeof(int));
        MPI_Recv(weights, numV * numV, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);

        spaths = allocate(numV * nodes * sizeof(int));

        dijkstra(&spaths, weights, numV, nodes, offset);

        MPI_Send(&nodes, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&offset, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(spaths, numV * nodes, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    exit(EXIT_SUCCESS);
}