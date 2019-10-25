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

    // MPI init should not return 0 if it was successful
    if (MPI_Init(&argc, &argv) != 0) {
        fprintf(stderr, "error initialising MPI");
        exit(EXIT_FAILURE);
    }
    
    // Gets the number of tasks and the current taskid to determine master
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    numworkers = numtasks - 1; // Exclude master worker

    if (taskid == MASTER) {
        process_graph(fp, &weights, &numV); // Read the fp and store the values accordingly
        spaths = allocate(numV * numV * sizeof(int));

        // Variables for workload balancing
        int avgV = numV / numworkers;
        int extra = numV % numworkers;

        // Start the timer before Dijkstra computation
        gettimeofday(&start, NULL); 
        
        // Send tasks to workers
        // Broadcast to all nodes variables that aren't node independant, O(logp)
        MPI_Bcast(&numV, 1, MPI_INT, MASTER, MPI_COMM_WORLD); 
        MPI_Bcast(weights, numV * numV, MPI_INT, MASTER, MPI_COMM_WORLD);
        for (int dest = 1; dest <= numworkers; dest++) {
             // Optimal way for workload split
            nodes = avgV + (dest <= extra); // Sends extra tasks to earlier workers
            
            // Sends the appropriate variables to the workers
            MPI_Send(&nodes, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            offset += numV * nodes;
        }

        // Receive tasks from workers
        for (int source = 1; source <= numworkers; source++) {
            MPI_Recv(&nodes, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            MPI_Recv(&offset, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            MPI_Recv(&spaths[offset], numV * nodes, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
        }

        gettimeofday(&end, NULL);

        // Output code
        char *filename = get_filename(argv[2]);
        FILE *fout = fopen(filename, "w");
        if (fout == NULL) {
            fprintf(stderr, "spath: failed to write to the output file\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            exit(EXIT_FAILURE);
        }
        // Binary output write, unspecified in project
        output(fout, spaths, numV);
        output_time(start, end);

        fclose(fp);
        fclose(fout);
        free(spaths);
        free(weights);
        free(filename);
    } else { // Worker code below
        // Receives variables and work from master
        MPI_Bcast(&numV, 1, MPI_INT, FROM_MASTER, MPI_COMM_WORLD);
        weights = allocate(numV * numV * sizeof(int)); // Allocate memory before receiving weights data
        MPI_Bcast(weights, numV * numV, MPI_INT, MASTER, MPI_COMM_WORLD);

        MPI_Recv(&nodes, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        spaths = allocate(numV * nodes * sizeof(int)); // Allocate only the required task

        dijkstra(&spaths, weights, numV, nodes, offset);

        // Sends finished work back to master
        MPI_Send(&nodes, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&offset, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(spaths, numV * nodes, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
    }
    MPI_Finalize(); // Called by master thread that MPI initialised
    exit(EXIT_SUCCESS);
}