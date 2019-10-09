// The program will by default output results to an output file

#include "spath.h"

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

    int *weights;
    int numV;
    int *spaths;
    int nodes, offset = 0;

    // MPI initialisation
    MPI_Status status;
    int numtasks, numworkers, taskid;

    if (MPI_Init(&argc, &argv) != 0) {
        fprintf(stderr, "error initialising MPI");
        exit(EXIT_FAILURE);
    }
    
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    numworkers = numtasks - 1;

    if (taskid == MASTER) {
        process_graph(fp, &weights, &numV);
        spaths = allocate(numV * numV * sizeof(int));

        int avgV = numV / numworkers;
        int extra = numV % numworkers;

        pint(avgV);
        pint(extra);

        // Send tasks to workers
        for (int dest = 1; dest <= numworkers; dest++) {
            pint(dest);
            nodes = avgV + (dest <= extra); // Split extra work
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

        // Output
        char *filename = get_filename(argv[2]);
        FILE *fout = fopen(filename, "w");
        if (fout == NULL) {
            fprintf(stderr, "spath: failed to write to the output file\n");
            exit(EXIT_FAILURE);
        }
        output(fout, spaths, numV);

        // Memory clearing
        fclose(fp);
        fclose(fout);
        free(spaths);
        free(weights);
        free(filename);
    } else {
        // Receive work from MASTER
        MPI_Recv(&nodes, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&numV, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        weights = allocate(numV * numV * sizeof(int));
        MPI_Recv(weights, numV * numV, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);

        printf("taskid=%d|numV=%d|offset=%d\n", taskid, numV, offset);

        spaths = allocate(numV * nodes * sizeof(int));
        for (int i = 0; i < nodes; i++) {
            dijkstra(offset/numV + i, weights, numV, i * numV, &spaths);
        }
        // Send results to MASTER
        MPI_Send(&nodes, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&offset, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(spaths, numV * nodes, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    exit(EXIT_SUCCESS);
}