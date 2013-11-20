#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int size, rank, i, dest, source, sum, temp_val;
    MPI_Status* status;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    dest = (rank + 1) % size;
    source = (rank - 1) % size;
    sum = temp_val = rank; 
    for(i = 1; i < size; i++){
        MPI_Sendrecv_replace(&temp_val, 1, MPI_INT, dest, 0, source, 0, MPI_COMM_WORLD, status); 
        sum += temp_val ;
    }

    printf("Process %d: %d\n", rank, sum);

    MPI_Finalize();

    return EXIT_SUCCESS;

}
