#include <stdio.h>
#include <mpi.h>

#define SIZE 24

int main(int argc, char* argv[]){
    
	MPI_Init(&argc, &argv);
	int myid, numprocs;
	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	if(numprocs > SIZE){ MPI_Finalize(); return 0; }
	/* init matrix A and vector x */
	int A[SIZE][SIZE];
	int x[SIZE];
	int i,j;
	if(myid == 0){
		for(i=0;i<SIZE;++i){
			for(j=0;j<SIZE;++j){
				A[i][j] = (i*SIZE+j) % 3;
			}
			x[i] = (SIZE-i) % 3;
		}
	}

	/*************************/
	/* YOUR TASK STARTS HERE */	
	/*************************/
        int result[SIZE];
	//distribute data
        if (myid == 0) {
            MPI_Bcast(&A, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&x, SIZE, MPI_INT, 0, MPI_COMM_WORLD);
        } else {
            MPI_Status* status = 0;
            MPI_Recv(&A, SIZE * SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, status);
            MPI_Recv(&x, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, status);
        }

	//calculate local values
        int breadth = SIZE / numprocs;
        for (i = 0; i < SIZE; i++) {
            result[i] = 0;
            for (j = myid * breadth; j < (myid + 1) * breadth; j++) {
                result[i] += A[i][j] * x[j];
            }
        }
	
	//collect data
        int new_result[SIZE];
        if (myid == 0) {
            MPI_Reduce(&result[i], &new_result[i], 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        }
	
	
	/***********************/
	/* YOUR TASK ENDS HERE */
	/***********************/

	/* print result vector */	
	if(myid == 0){
		for(i=0;i<SIZE;++i){
			printf("%d ", new_result[i]);
		}
		printf("\n");
	}

	MPI_Finalize(); 
	return 0;
}
